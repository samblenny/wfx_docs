#include <stdint.h>
#include <stdbool.h>
#include "sl_wfx.h"
#include "dbg.h"
#include "m4_spi.h"

#ifndef NOT_ARDUINO
#include "Arduino.h"
#endif

/*
 * Declare WF200 Firmware & BRD8022A PDS table to be linked against ROM
 */
extern const uint32_t sl_wfx_firmware_size;
extern const uint8_t sl_wfx_firmware[];
extern const char* const *pds_table;
extern const uint16_t pds_table_lines;

/*
 * Implement WFX driver's debug API
 */
#ifdef SL_WFX_DEBUG_MASK
#include <stdarg.h> // for sl_wfx_host_log va_* stuff
void sl_wfx_host_log(const char *string, ...)
{
    // Pass sl_wfx debug logging straight to vprintf
    va_list ap;
    va_start(ap, string);
    vprintf(string, ap);
    va_end(ap);
}
#endif

// Memory pool for host_allocate_buffer
#define ALLOC_POOL_SIZE 1024
static uint8_t _alloc_pool[ALLOC_POOL_SIZE];
static uint8_t *_alloc_last_ptr = NULL;
static uint32_t _alloc_last = 0;
static uint32_t _alloc_next = 0;
static uint32_t _alloc_highwater_mark = 0;

// Driver context
sl_wfx_context_t *_context_ptr = NULL;

/*
 * Non-wfx_host_api functions to help with debug tracing
 */

void remember_context_ptr(sl_wfx_context_t *context) {
    _context_ptr = context;
}

/*
 * sl_wfx_* API function implementations
 */

sl_status_t sl_wfx_host_init(void)
{
    dbg("init\n");
    memset(_alloc_pool, 0, ALLOC_POOL_SIZE);
    _alloc_next = 0;
    return m4_init();
}

sl_status_t sl_wfx_host_get_firmware_data(
    const uint8_t **data,
    uint32_t data_size)
{
    static const uint8_t *seek_ptr = NULL;
    if(data == NULL) {
        return SL_STATUS_FAIL;
    }
    const uint8_t* start = sl_wfx_firmware;
    const uint8_t* end = sl_wfx_firmware + sl_wfx_firmware_size;
    if(*data == NULL) {
        dbg("get_firmware(*data:NULL, data_size:");
    } else {
        dbg("get_firmware(*data-start:");
        dbg_u32((uint32_t)(*data-start));
        dbg(", data_size:");
    }
    dbg_u32(data_size);
    dbg(")");
    if(*data == NULL && start + data_size <= end) {
        // Seek to start of firmware image (keyset)
        *data = start;
        seek_ptr = start + data_size;
    } else if(*data >= start && seek_ptr + data_size <= end) {
        // Advance seek pointer from previous position
        *data = seek_ptr;
        seek_ptr += data_size;
    } else {
        // Fail: attempt to seek past end
        dbg(" -> FAIL\n");
        return SL_STATUS_FAIL;
    }
    dbg(" -> OK(*data-start=");
    dbg_u32((uint32_t)(*data-start));
    dbg(")\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_get_firmware_size(uint32_t *firmware_size)
{
    dbg("firmware_size: ");
    dbg_u32(sl_wfx_firmware_size);
    dbg("\n");
    *firmware_size = sl_wfx_firmware_size;
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_get_pds_data(const char **pds_data, uint16_t index)
{
    dbg("get_pds_data");
    if(pds_data == NULL) {
        dbg(": *pds_data == NULL\n");
        return SL_STATUS_FAIL;
    }
    // pds_table is an array of strings, one line per string
    dbg("(*pds_data:..., index:");
    dbg_u32(index);
    dbg(")");
    if(index < pds_table_lines) {
        // Get a line of the PDS array as requested by index
        *pds_data = pds_table[index];
    } else {
        dbg(" -> FAIL(index >= lines)\n");
        return SL_STATUS_FAIL;
    }
    dbg(" -> OK(*pds_data-start=");
    dbg_u32((uint32_t)(*pds_data-pds_table[0]));
    dbg(")\n");
    return SL_STATUS_OK;
}

// Get number of lines in the PDS (pds is an array of strings, one line per string)
sl_status_t sl_wfx_host_get_pds_size(uint16_t *pds_size)
{
    dbg("get_pds_size: ");
    if(pds_size == NULL) {
        dbg("*pds_size == NULL\n");
        return SL_STATUS_FAIL;
    }
    *pds_size = pds_table_lines;
    dbg_u16(*pds_size);
    dbg(" lines\n");
    return SL_STATUS_OK;
}

/*
 * NOTE: This is sorta tricky as it only gets called as part of the error
 * handler for sl_wfx_init(). You might expect it would get called as part of
 * sl_wfx_deinit(), but you'd be wrong. For that, try sl_wfx_host_deinit_bus().
 */
sl_status_t sl_wfx_host_deinit(void)
{
    dbg("deinit() -> OK\n");
    return m4_deinit();
}

sl_status_t sl_wfx_host_reset_chip(void)
{
    sl_status_t status = m4_reset_chip();
    dbg("reset_chip\n");
    return status;
}

sl_status_t sl_wfx_host_set_wake_up_pin(uint8_t state)
{
    dbg("set_wake_up_pin(state:");
    dbg_u8(state);
    dbg(") -> OK\n");
    m4_set_wup(state);
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_wake_up(void)
{
    dbg("wait_for_wake_up\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_sleep_grant(
    sl_wfx_host_bus_transfer_type_t type,
    sl_wfx_register_address_t address,
    uint32_t length)
{
    dbg("sleep_grant\n");
    return SL_STATUS_WIFI_SLEEP_NOT_GRANTED;
}

sl_status_t sl_wfx_host_setup_waited_event(uint8_t event_id)
{
    dbg("setup_waited_event(event_id:");
    dbg_u8(event_id);
    dbg(") -> OK\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_confirmation(
    uint8_t confirmation_id,
    uint32_t timeout_ms,
    void **event_payload_out)
{
    sl_status_t status;
    uint16_t ctrl_reg;
    // TODO: The real event payload should come from the queue for events from
    //   sl_wfx_host_post_event().
    //   Use *event_payload_out = sl_wfx_context->event_payload_buffer
    dbg("wait_for_confirmation(conf_id:");
    dbg_u8(confirmation_id);
    dbg(", timeout_ms:");
    dbg_u32(timeout_ms);
    dbg(", ...) -> ");
    if(event_payload_out == NULL) {
        dbg("FAIL(event_payload_out == NULL)\n");
        return SL_STATUS_FAIL;
    }
    dbg("FAIL(unimplemented)\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_wait(uint32_t wait_ms)
{
    dbg("wait ");
    dbg_u32(wait_ms);
    dbg("ms\n");
    m4_wait(wait_ms);
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_post_event(sl_wfx_generic_message_t *event_payload)
{
    dbg("post_event\n");
    // TODO: post event
    return SL_STATUS_FAIL;
}

// Allocate a buffer
sl_status_t sl_wfx_host_allocate_buffer(
    void **buffer,
    sl_wfx_buffer_type_t type,
    uint32_t buffer_size)
{
    dbg("alloc");
    if(buffer == NULL) {
        dbg(": buffer == NULL\n");
        return SL_STATUS_FAIL;
    }
    dbg("(**bufer:..., type:");
    switch(type) {
    case SL_WFX_TX_FRAME_BUFFER:
        dbg("TX_FRAME_BUFFER");
        break;
    case SL_WFX_RX_FRAME_BUFFER:
        dbg("RX_FRAME_BUFFER");
        break;
    case SL_WFX_CONTROL_BUFFER:
        dbg("CONTROL_BUFFER");
        break;
    default:
        dbg("???");
    }
    dbg(", buffer_size:");
    dbg_u32(buffer_size);
    dbg(") -> ");
    // Allocate a chunk of the pool
    if(_alloc_next + buffer_size < ALLOC_POOL_SIZE) {
        *buffer = &_alloc_pool[_alloc_next];
        // Save pointer and index in case buffer is freed before next alloc
        _alloc_last_ptr = *buffer;
        _alloc_last = _alloc_next;
        dbg("OK(*buffer=&pool[");
        dbg_u32(_alloc_next);
        // Calculate index to start of next allocatable block of pool
        _alloc_next += buffer_size;
        // Calculate high-water mark (index of highest allocated byte of pool)
        if(_alloc_next > 0 && _alloc_next > _alloc_highwater_mark+1) {
            _alloc_highwater_mark = _alloc_next - 1;
        }
        dbg("])\n");
        return SL_STATUS_OK;
    } else {
        dbg("FAIL(pool too small)\n");
        return SL_STATUS_FAIL;
    }
}

// Free a buffer
sl_status_t sl_wfx_host_free_buffer(void *buffer, sl_wfx_buffer_type_t type)
{
    dbg("free(*buffer:");
    if(buffer == NULL) {
        dbg(" NULL) -> FAIL\n");
        return SL_STATUS_FAIL;
    }
    if(_alloc_last_ptr == NULL || _alloc_last < 0 || _alloc_last >= ALLOC_POOL_SIZE) {
        // Fail for illegal values of _alloc_last_ptr or _alloc_last
        dbg(" ?) -> FAIL(last_ptr-&pool: ");
        dbg_u32(_alloc_last_ptr-_alloc_pool);
        dbg(", last: ");
        dbg_u32(_alloc_last);
        dbg(")\n");
        return SL_STATUS_FAIL;
    }
    if(buffer == _alloc_last_ptr) {
        // For free() of last alloc(), zero the buffer and return it to pool
        uint32_t size = _alloc_next - _alloc_last;
        memset((void*)_alloc_last_ptr, 0, size);
        // Adjust start of next available block of the pool, intentionally
        // allowing _alloc_last to keep a now invalid value (see next comment)
        _alloc_next = _alloc_last;
        // Setting _alloc_last_ptr = NULL guards against double-free
        _alloc_last_ptr = NULL;
        dbg(" ==last) -> OK(next: pool[");
    } else {
        // Otherwise, allow buffer to leak
        dbg(" !=last) -> OK(leaking, next: pool[");
    }
    dbg_u32(_alloc_next);
    dbg("])\n");
    return SL_STATUS_OK;
}

// Transmit frame
sl_status_t sl_wfx_host_transmit_frame(void *frame, uint32_t frame_len)
{
    dbg("tx_frame(*frame:..., frame_len:");
    dbg_u32(frame_len);
    dbg(")...\n "); // End debug line so it doesn't mix with sl_wfx_data_write
    sl_status_t status;
    status = sl_wfx_data_write(frame, frame_len);
    if(status == SL_STATUS_OK) {
        dbg(" ...tx_frame -> OK\n");
    } else {
        dbg(" ...tx_frame -> FAIL(status:0x)");
        dbg_hex32(status);
        dbg("\n");
    }
    return status;
}

sl_status_t sl_wfx_host_lock(void)
{
    dbg("lock() -> OK\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_unlock(void)
{
    dbg("unlock() -> OK\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_init_bus(void)
{
    dbg("init_bus\n");
    m4_init_bus();
    sl_wfx_host_set_wake_up_pin(1);
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_deinit_bus(void)
{
    dbg("deinit_bus() -> OK(_alloc_highwater_mark: ");
    dbg_u32(_alloc_highwater_mark);
    dbg(", context->used_buffers: ");
    if(_context_ptr == NULL) {
        dbg("NULL");
    } else {
        dbg_u16(_context_ptr->used_buffers);
    }
    dbg(")\n");
    m4_deinit_bus();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_enable_platform_interrupt(void)
{
    dbg("irq=on\n");
    m4_enable_interrupt();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_disable_platform_interrupt(void)
{
    dbg("irq=off\n");
    m4_disable_interrupt();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_assert(void)
{
    m4_cs_assert();
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_deassert(void)
{
    m4_cs_deassert();
    return SL_STATUS_OK;
}

// Decode base-16 ASCII digit to 0..15 binary equivalent
uint32_t decode_nibl(uint8_t nib) {
    if(nib >= (uint8_t)'0' && nib <= (uint8_t)'9') {
        return (uint32_t)(nib - (uint8_t)'0');
    } else if(nib >= (uint8_t)'a' && nib <= (uint8_t)'f') {
        return (uint32_t)(nib - (uint8_t)'a' + 10);
    } else if(nib >= (uint8_t)'A' && nib <= (uint8_t)'F') {
        return (uint32_t)(nib - (uint8_t)'A' + 10);
    }
    return 0;
}

void pack_4xu8_mode0(uint8_t buffer[4], uint32_t val) {
    buffer[2] = (uint8_t)(val>>24);
    buffer[3] = (uint8_t)((val<<8)>>24);
    buffer[0] = (uint8_t)((val<<16)>>24);
    buffer[1] = (uint8_t)((val<<24)>>24);
}

void pack_4xu8_mode2(uint8_t buffer[4], uint32_t val) {
    buffer[3] = (uint8_t)(val>>24);
    buffer[2] = (uint8_t)((val<<8)>>24);
    buffer[1] = (uint8_t)((val<<16)>>24);
    buffer[0] = (uint8_t)((val<<24)>>24);
}

sl_status_t sl_wfx_host_spi_transfer_no_cs_assert(
    sl_wfx_host_bus_transfer_type_t type,
    uint8_t *header,
    uint16_t header_length,
    uint8_t *buffer,
    uint16_t buffer_length)
{
    // State variables for keeping in sync with wfx driver
    static uint32_t sram_base_address = 0;
    static uint32_t ncp_status_tick = 0;
    static uint32_t ctrl_area_put = 0;
    static bool download_complete = false;

    // Debug print the raw byte array arguments
    dbg("SPI(0x");
    for(int i=0; i < header_length; i++) {
        dbg_hex8(header[i]);
    }
    if(buffer_length <= 32) {
        dbg(", 0x");
        for(int i=0; i < buffer_length; i++) {
            dbg_hex8(buffer[i]);
        }
    } else {
        dbg(" <buffer_length:");
        dbg_u32(buffer_length);
        dbg(" bytes>");
    }
    dbg(") ");

    // Do the hardware SPI transfer
    bool read;
    if(type == SL_WFX_BUS_WRITE) {
        read = false;
    } else if(type == SL_WFX_BUS_READ) {
        read = true;
    } else {
        // At the time I'm writing this, wfx-fullMAC-driver declares an
        // SL_WFX_BUS_WRITE_AND_READ enum value that is not used by any driver
        // code. That's good, because it would be difficult to implement
        // efficiently with the hardware SPI peripheral.
        dbg("SPI transfer -> FAIL(unsupported transfer type)\n");
        return SL_STATUS_FAIL;
    }
    m4_spi_transfer(read, header, header_length, buffer, buffer_length);

    // All transfers should start with a 16-bit command word (the header), and
    // they often have a transfer length of two 16-bit words, (buffer_length ==
    // 4). The order in which bytes get arranged in the data buffer varries
    // based on the word mode.
    //
    // This attempts to interpret what was transferred.
    if(header_length == 2 && buffer_length == 4) {
        uint16_t cmd = ((uint16_t)(header[0]))<<8 | (uint16_t)(header[1]);
        char cmd_rw = (cmd>>15 == 1) ? 'R' : 'W';
        uint16_t cmd_addr = cmd>>12 & 0x7;
        uint32_t buf = 0;
        if(cmd_addr == SL_WFX_CONFIG_REG_ID) {
            // Config reg at address 0 always uses word Mode0 (B1,B0,B3,B2)
            buf = ((uint32_t)(buffer[2]))<<24 | ((uint32_t)(buffer[3]))<<16;
            buf |= ((uint32_t)(buffer[0]))<<8 | (uint32_t)(buffer[1]);
        } else {
            // sl_wfx_bus_spi.c:sl_wfx_init_bus() sets word Mode2, which means
            // bytes get transferred in little endian order: B0,B1,B2,B3
            buf = ((uint32_t)(buffer[3]))<<24 | ((uint32_t)(buffer[2]))<<16;
            buf |= ((uint32_t)(buffer[1]))<<8 | (uint32_t)(buffer[0]);
        }
        dbg("-> ");
        uint32_t width = 25;
        if(cmd_rw == 'W') {
            dbg("W(");
            dbg_decode_addr(cmd_addr);
            dbg("=");
            dbg_decode_u32(buf, width);
            dbg(")");
        } else {
            dbg("R(");
            dbg_decode_addr(cmd_addr);
            dbg_w("", width+1);
            dbg(")");
        }
        // Debug print the NOT-packed output value of buffer
        dbg(" -> OK(");
        if(cmd_rw == 'R') { dbg_decode_u32(buf, 10); }
        else              { dbg_hex32(buf); }
        dbg(")\n");
    } else {
        // Since (header_length == 2 && buffer_length == 4) was false, this is
        // not a ctrl/config register access. So, use simpler debug.
        dbg("-> OK\n");
    }
    return SL_STATUS_OK;
}

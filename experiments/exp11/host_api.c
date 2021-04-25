#include <stdint.h>
#include <stdbool.h>
#include "sl_wfx.h"
#include "dbg.h"
/* Declare WF200 Firmware & BRD8022A PDS table to be linked against ROM
 */
extern const uint32_t sl_wfx_firmware_size;
extern const uint8_t sl_wfx_firmware[];
extern const char* const *pds_table;

#ifdef SL_WFX_DEBUG_MASK
#include <stdarg.h> // for sl_wfx_host_log va_* stuff

/*
 * Pass sl_wfx debug logging straight to vprintf
 */
void sl_wfx_host_log(const char *string, ...)
{
    va_list ap;
    va_start(ap, string);
    vprintf(string, ap);
    va_end(ap);
}
#endif

/**
 * Static data (avoid malloc)
 */

// _startup_info:
// 1. Is used by sl_wfx_host_wait_for_confirmation() to fake the event payload
//    for SL_WFX_STARTUP_IND_ID.
// 2. TODO: get rid of this and change sl_wfx_host_wait_for_confirmation() to
//    get events from the queue for sl_wfx_host_post_event()
static sl_wfx_startup_ind_t _startup_info;

// _configuration_reply is used by sl_wfx_send_configuration()
static sl_wfx_configuration_cnf_t _configuration_reply;

// Memory pool for host_allocate_buffer
#define ALLOC_POOL_SIZE 1024
static uint8_t _alloc_pool[ALLOC_POOL_SIZE];
static uint32_t _alloc_next = 0;

/*
 * sl_wfx_* API function implementations
 */

sl_status_t sl_wfx_host_init(void)
{
    dbg("init\n");
    memset(_alloc_pool, 0, ALLOC_POOL_SIZE);
    _alloc_next = 0;
    return SL_STATUS_OK;
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
    const uint16_t lines = sizeof(pds_table) / sizeof(const char *);
    dbg("(*pds_data:..., index:");
    dbg_u32(index);
    dbg(")");
    if(index < lines) {
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
    *pds_size = sizeof(pds_table) / sizeof(const char *);
    dbg_u16(*pds_size);
    dbg(" lines\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_deinit(void)
{
    dbg("deinit\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_reset_chip(void)
{
    dbg("reset_chip\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_set_wake_up_pin(uint8_t state)
{
    dbg("set_wake_up_pin\n");
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
    switch(confirmation_id) {
    case SL_WFX_STARTUP_IND_ID:
        // TODO: use a real event payload buffer from WF200
        dbg("OK(SL_WFX_STARTUP_IND_ID)\n");
        memset(&_startup_info, 0, sizeof(sl_wfx_startup_ind_t));
        // See typedefs at firmware/sl_wfx_general_api.h#L305..L332
        _startup_info.body.firmware_build = 3;
        _startup_info.body.firmware_major = 12;
        _startup_info.body.firmware_minor = 2;
        _startup_info.body.num_inp_ch_bufs = 6;
        *event_payload_out = &_startup_info;
        return SL_STATUS_OK;
    case SL_WFX_CONFIGURATION_REQ_ID:
        dbg("OK(CONFIGURATION_REQ_ID)\n");
        memset(&_configuration_reply, 0, sizeof(sl_wfx_configuration_cnf_t));
        _configuration_reply.body.status = SL_STATUS_OK;
        *event_payload_out = &_configuration_reply;
        return SL_STATUS_OK;
    case SL_WFX_CONTROL_GPIO_REQ_ID:
        dbg("CONTROL_GPIO_REQ_ID ");
        break;
    case SL_WFX_PREVENT_ROLLBACK_REQ_ID:
        dbg("PREVENT_ROLLBACK_REQ_ID ");
        break;
    case SL_WFX_PTA_SETTINGS_REQ_ID:
        dbg("PTA_SETTINGS_REQ_ID ");
        break;
    case SL_WFX_PTA_PRIORITY_REQ_ID:
        dbg("PTA_PRIORITY_REQ_ID ");
        break;
    case SL_WFX_PTA_STATE_REQ_ID:
        dbg("PTA_STATE_REQ_ID ");
        break;
    case SL_WFX_SET_CCA_CONFIG_REQ_ID:
        dbg("SET_CCA_CONFIG_REQ_ID ");
        break;
    case SL_WFX_SHUT_DOWN_REQ_ID:
        dbg("SHUT_DOWN_REQ_ID ");
        break;
    default:
        dbg("FAIL(unknown confirmation_id)\n");
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
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_post_event(sl_wfx_generic_message_t *event_payload)
{
    dbg("post_event\n");
    return SL_STATUS_FAIL;
}

// Allocate a buffer
sl_status_t sl_wfx_host_allocate_buffer(
    void **buffer,
    sl_wfx_buffer_type_t type,
    uint32_t buffer_size)
{
    dbg("allocate_buffer");
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
        dbg("OK(*buffer=&_alloc_pool[");
        dbg_u32(_alloc_next);
        _alloc_next += buffer_size;
        dbg("])\n");
        return SL_STATUS_OK;
    } else {
        dbg("FAIL(pool too small)\n");
        return SL_STATUS_FAIL;
    }
}

// Free a buffer (currently a NOP)
sl_status_t sl_wfx_host_free_buffer(void *buffer, sl_wfx_buffer_type_t type)
{
    dbg("free_buffer() -> OK\n");
    // TODO: improve upon just allowing buffer to leak
    return SL_STATUS_OK;
}

// Transmit frame
sl_status_t sl_wfx_host_transmit_frame(void *frame, uint32_t frame_len)
{
    dbg("transmit_frame(*frame:..., frame_len:");
    dbg_u32(frame_len);
    dbg(") -> OK\n");
    return SL_STATUS_OK;
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
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_deinit_bus(void)
{
    dbg("deinit_bus\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_enable_platform_interrupt(void)
{
    dbg("irq=on\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_disable_platform_interrupt(void)
{
    dbg("irq=off\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_assert(void)
{
    // dbg("cs ");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_deassert(void)
{
    // dbg("CS\n");
    dbg("\n");
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
        dbg(" -> ");
        if(cmd_rw == 'W' && cmd_addr == SL_WFX_SRAM_BASE_ADDR_REG_ID) {
            sram_base_address = buf;
        }
        else if(cmd_addr == SL_WFX_CONTROL_REG_ID)
        {
            // When sl_wfx_init_chip() writes control with WUP bit set, it
            // expects to see responses with RDY set, or else it goes into a
            // big polling loop. So, claim to be always RDY.
            buf |= SL_WFX_CONT_RDY_BIT;
            // Re-pack buffer (Mode2) with the output value of buf
            pack_4xu8_mode2(buffer, buf);
        }
        else if(cmd_addr == SL_WFX_CONFIG_REG_ID)
        {
            // This needs to be something other than 0x00000000 or 0xFFFFFFFF,
            // or else sl_wfx_init_bus() will abort with SL_STATUS_FAIL.
            // sl_wfx_init_chip() checks for SL_WFX_CONFIG_ACCESS_MODE_BIT,
            // so use that.
            buf = SL_WFX_CONFIG_ACCESS_MODE_BIT;
            // Re-pack buffer (config is always Mode0) with output value of buf
            pack_4xu8_mode0(buffer, buf);
        }
        else if(cmd_addr == SL_WFX_SRAM_DPORT_REG_ID)
        {
            // Lookup table of SRAM read responses & write actions
            switch(sram_base_address) {
            case ADDR_DWL_CTRL_AREA_NCP_STATUS:
                if(!download_complete) {
                    // sl_wfx_download_run_firmware() checks for these early on
                    switch(ncp_status_tick) {
                    case 0: break;
                    case 1: buf = NCP_STATE_INFO_READY; break;
                    case 2: buf = NCP_STATE_READY; break;
                    }
                    ncp_status_tick++;
                } else {
                    buf = NCP_STATE_AUTH_OK;
                }
                break;
            case ADDR_DOWNLOAD_FIFO_BASE:
                // sl_wfx_download_run_bootloader() checks for this
                buf = 0x23abc88e;
                break;
            case SL_WFX_PTE_INFO + 12:
                // sl_wfx_download_run_firmware() reads this expecting to get (firmware_keyset << 8)
                buf = decode_nibl(sl_wfx_firmware[6]) << 12 | decode_nibl(sl_wfx_firmware[7]) << 8;
                break;
            case ADDR_DWL_CTRL_AREA_PUT:
                // Save sl_wfx_download_run_firmware() fifo put block count for when it checks get
                if(cmd_rw == 'W') { ctrl_area_put = buf; }
                break;
            case ADDR_DWL_CTRL_AREA_GET:
                // Make sl_wfx_download_run_firmware()'s check happy by claiming fifo get == fifo put
                if(cmd_rw == 'R') { buf = ctrl_area_put; }
                break;
            case ADDR_DWL_CTRL_AREA_HOST_STATUS:
                if(cmd_rw == 'W' && buf == HOST_STATE_UPLOAD_COMPLETE) {
                    download_complete = true;
                }
                break;
            }
            // Re-pack buffer (Mode2) with the output value of buf
            pack_4xu8_mode2(buffer, buf);
        }
        // Debug print the NOT-packed output value of buffer
        dbg("OK(");
        if(cmd_rw == 'R') { dbg_decode_u32(buf, 10); }
        else              { dbg_hex32(buf); }
        dbg(") ");
    }
    return SL_STATUS_OK;
}

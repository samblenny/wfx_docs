#include <stdint.h>
#include <stdbool.h>
#include "sl_wfx.h"
#include "dbg.h"
#include "hal.h"
#include "state.h"

/*
 * Declare WF200 Firmware & BRD8022A PDS table to be linked against ROM
 */
extern const uint32_t sl_wfx_firmware_size;
extern const uint8_t sl_wfx_firmware[];
extern const char* const *pds_table;
extern const uint16_t pds_table_lines;

// Memory pool for host_allocate_buffer
#define ALLOC_POOL_SIZE 4096
static uint8_t _alloc_pool[ALLOC_POOL_SIZE];
static uint8_t *_alloc_curr_ptr = NULL;
static uint8_t *_alloc_prev_ptr = NULL;
static uint32_t _alloc_curr = 0;
static uint32_t _alloc_prev = 0;
static uint32_t _alloc_next = 0;
static uint32_t _alloc_highwater_mark = 0;

// Driver context
sl_wfx_context_t *_context_ptr = NULL;

// Host implementation context
uint8_t _waited_event_id;
uint8_t _posted_event_id;

/*
 * Non-wfx_host_api functions to help with debug tracing
 */

void remember_context_ptr(sl_wfx_context_t *context) {
    _context_ptr = context;
}

/*
 * sl_wfx_* API function implementations
 */

sl_status_t sl_wfx_host_init(void) {
    dbg("(init)\n");
    memset(_alloc_pool, 0, ALLOC_POOL_SIZE);
    _alloc_next = 0;
    return hal_init();
}

sl_status_t sl_wfx_host_get_firmware_data(
    const uint8_t **data,
    uint32_t data_size
) {
    static const uint8_t *seek_ptr = NULL;
    if(data == NULL) {
        return SL_STATUS_FAIL;
    }
    const uint8_t* start = sl_wfx_firmware;
    const uint8_t* end = sl_wfx_firmware + sl_wfx_firmware_size;
    if(*data == NULL) {
        dbg("(fw_dat NULL ");
    } else {
        dbg("(fw_dat ");
        dbg_u32((uint32_t)(*data-start));
        dbg(" ");
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
        dbg(" FAIL\n");
        return SL_STATUS_FAIL;
    }
    dbg(" -> (OK ");
    dbg_u32((uint32_t)(*data-start));
    dbg(")\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_get_firmware_size(uint32_t *firmware_size) {
    dbg("(fw_size) (OK ");
    dbg_u32(sl_wfx_firmware_size);
    dbg(")\n");
    *firmware_size = sl_wfx_firmware_size;
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_get_pds_data(const char **pds_data, uint16_t index) {
    dbg("(pds_dat ");
    dbg_hex32(pds_data);
    dbg(" ");
    dbg_u16(index);
    dbg(") ");
    if(pds_data == NULL) {
        dbg("(FAIL pds_data==NULL)\n");
        return SL_STATUS_FAIL;
    }
    // pds_table is an array of strings, one line per string
    if(index < pds_table_lines) {
        // Get a line of the PDS array as requested by index
        *pds_data = pds_table[index];
    } else {
        dbg("(FAIL index>=lines)\n");
        return SL_STATUS_FAIL;
    }
    dbg(" (OK");
    dbg_hex32((uint32_t)(*pds_data));
    dbg(")\n");
    return SL_STATUS_OK;
}

// Get number of lines in the PDS table (array of strings, 1 line per string)
sl_status_t sl_wfx_host_get_pds_size(uint16_t *pds_size) {
    dbg("(pds_size) ");
    if(pds_size == NULL) {
        dbg("(FAIL *pds_size==NULL)\n");
        return SL_STATUS_FAIL;
    }
    *pds_size = pds_table_lines;
    dbg_u16(*pds_size);
    dbg(" lines)\n");
    return SL_STATUS_OK;
}

/*
 * NOTE: This is sorta tricky as it only gets called as part of the error
 * handler for sl_wfx_init(). You might expect it would get called as part of
 * sl_wfx_deinit(), but you'd be wrong. For that, try sl_wfx_host_deinit_bus().
 */
sl_status_t sl_wfx_host_deinit(void) {
    dbg("deinit\n");
    return hal_deinit();
}

sl_status_t sl_wfx_host_reset_chip(void) {
    dbg("reset\n");
    return hal_reset_chip();
}

sl_status_t sl_wfx_host_set_wake_up_pin(uint8_t state) {
    dbg("(set_wup ");
    dbg_u8(state);
    dbg(")\n");
    return hal_set_wup(state);
}

sl_status_t sl_wfx_host_wait_for_wake_up(void) {
    dbg("wait_for_wake_up\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_sleep_grant(
    sl_wfx_host_bus_transfer_type_t type,
    sl_wfx_register_address_t address,
    uint32_t length
) {
    dbg("sleep_grant\n");
    return SL_STATUS_WIFI_SLEEP_NOT_GRANTED;
}

sl_status_t sl_wfx_host_setup_waited_event(uint8_t event_id) {
    dbg("(wait_ev ");
    dbg_message_id(event_id);
    dbg(") ");
    if(_context_ptr == NULL) {
        dbg("(FAIL _context_ptr==NULL)\n");
        return SL_STATUS_FAIL;
    }
    _waited_event_id = event_id;
    dbg("OK\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_confirmation(
    uint8_t confirmation_id,
    uint32_t timeout_ms,
    void **event_payload_out
) {
    if(event_payload_out == NULL) {
        dbg("(wait_conf event_payload_out==NULL) FAIL\n");
        return SL_STATUS_FAIL;
    }
    if(_context_ptr == NULL) {
        dbg("(wait_conf _context_ptr==NULL) FAIL\n");
        return SL_STATUS_FAIL;
    }
    sl_status_t result;
    uint16_t ctrl_reg = 0;
    dbg("(wait_conf ");
    dbg_message_id(confirmation_id);
    dbg(" ");
    dbg_u32(timeout_ms);
    dbg(")\n");
    for(uint32_t t=0; t<timeout_ms; t++) {
        if(!hal_wirq_asserted()) {
            hal_wait(1);
            continue;
        }
        result = sl_wfx_receive_frame(&ctrl_reg);
        if(result != SL_STATUS_OK) {
            dbg("(FAIL wait_conf status:");
            dbg_hex32(result);
            dbg(")\n");
            return result;
        }
        sl_wfx_host_setup_waited_event(confirmation_id);
        if(_posted_event_id == confirmation_id) {
            _posted_event_id = 0;
            *event_payload_out = _context_ptr->event_payload_buffer;
            dbg("(OK wait_conf)\n");
            switch(confirmation_id) {
            case SL_WFX_STARTUP_IND_ID:
                dbg_startup_ind(*event_payload_out);
                break;
            }
            return SL_STATUS_OK;
        }
    }
    dbg("(FAIL wait_conf TIMEOUT)\n");
    return SL_STATUS_TIMEOUT;
}

sl_status_t sl_wfx_host_wait(uint32_t wait_ms) {
    dbg("wait ");
    dbg_u32(wait_ms);
    dbg("ms\n");
    return hal_wait(wait_ms);
}

sl_status_t sl_wfx_host_post_event(sl_wfx_generic_message_t *event_payload) {
    if(event_payload == NULL) {
        dbg("(post event_payload==NULL) FAIL\n");
        return SL_STATUS_FAIL;
    }
    if(_context_ptr == NULL) {
        dbg("(post _context_ptr==NULL) FAIL\n");
        return SL_STATUS_FAIL;
    }
    bool generic_dbg = false;
    switch(event_payload->header.id) {
    case SL_WFX_SCAN_RESULT_IND_ID:
        dbg_scan_result((sl_wfx_scan_result_ind_body_t *)(event_payload->body));
        break;
    case SL_WFX_START_SCAN_CNF_ID:
        state_set_ssid_scanning(true);
        break;
    case SL_WFX_SCAN_COMPLETE_IND_ID:
        state_set_ssid_scanning(false);
        break;
    case SL_WFX_STARTUP_IND_ID:       // fall through
    case SL_WFX_CONFIGURATION_REQ_ID: // fall through
    case SL_WFX_STOP_SCAN_CNF_ID:
        // Generic debug message for OK but no handler function needed
        dbg("(post ");
        dbg_message_id(event_payload->header.id);
        dbg(") OK\n");
        break;
    default:
        dbg("(post unimplemented ");
        dbg_hex16(event_payload->header.id);
        dbg(") FAIL\n");
        return SL_STATUS_FAIL;
    }
    // Copy the event payload to the buffer array in the wfx driver context
    memcpy(_context_ptr->event_payload_buffer, (void *)event_payload,
        event_payload->header.length);
    _posted_event_id = event_payload->header.id;
    return SL_STATUS_OK;
}

// Allocate a buffer
sl_status_t sl_wfx_host_allocate_buffer(
    void **buffer,
    sl_wfx_buffer_type_t type,
    uint32_t buffer_size
) {
#ifndef DBG_HIDE_ALLOC_FREE
    dbg("(alloc ");
    dbg_hex32((uint32_t)buffer);
    dbg(" ");
    dbg_buffer_type(type);
    dbg(" ");
    dbg_u32(buffer_size);
    dbg(") ");
#endif
    if(buffer == NULL) {
#ifndef DBG_HIDE_ALLOC_FREE
        dbg("(FAIL buffer==NULL) ");
        dbg_alloc_stack(_alloc_curr_ptr, _alloc_prev_ptr);
        dbg("\n");
#endif
        return SL_STATUS_FAIL;
    }
    // Allocate a chunk of the pool
    if(_alloc_next + buffer_size < ALLOC_POOL_SIZE) {
        *buffer = &_alloc_pool[_alloc_next];
        // Save pointer and index in case buffer is freed before next alloc
        _alloc_prev_ptr = _alloc_curr_ptr;
        _alloc_curr_ptr = *buffer;
        _alloc_prev = _alloc_curr;
        _alloc_curr = _alloc_next;
#ifndef DBG_HIDE_ALLOC_FREE
        dbg("(OK ");
        dbg_hex32((uint32_t)(*buffer));
        dbg(") ");
        dbg_alloc_stack(_alloc_curr_ptr, _alloc_prev_ptr);
        dbg("\n");
#endif
        // Calculate index to start of next allocatable block of pool
        _alloc_next += buffer_size;
        // Calculate high-water mark (index of highest allocated byte of pool)
        if(_alloc_next > 0 && _alloc_next > _alloc_highwater_mark+1) {
            _alloc_highwater_mark = _alloc_next - 1;
        }
        return SL_STATUS_OK;
    } else {
#ifndef DBG_HIDE_ALLOC_FREE
        dbg("(FAIL pool_too_small) ");
        dbg_alloc_stack(_alloc_curr_ptr, _alloc_prev_ptr);
        dbg("\n");
#endif
        return SL_STATUS_FAIL;
    }
}

// Free a buffer
sl_status_t sl_wfx_host_free_buffer(void *buffer, sl_wfx_buffer_type_t type) {
#ifndef DBG_HIDE_ALLOC_FREE
    dbg("(free ");
    dbg_hex32((uint32_t)buffer);
    dbg(" ");
    dbg_buffer_type(type);
    dbg(") ");
    dbg_alloc_stack(_alloc_curr_ptr, _alloc_prev_ptr);
    dbg(" ");
#endif
    if(buffer == NULL) {
#ifndef DBG_HIDE_ALLOC_FREE
        dbg("(FAIL buffer==NULL)\n");
#endif
        return SL_STATUS_FAIL;
    }
    if(_alloc_curr_ptr == NULL || _alloc_curr < 0 || _alloc_curr >= ALLOC_POOL_SIZE) {
        // Fail for illegal values of _alloc_curr_ptr or _alloc_curr
#ifndef DBG_HIDE_ALLOC_FREE
        dbg("(FAIL confused) ");
        dbg_alloc_stack(_alloc_curr_ptr, _alloc_prev_ptr);
        dbg("\n");
#endif
        return SL_STATUS_FAIL;
    }
    if(buffer == _alloc_curr_ptr) {
        // For free() of last alloc(), zero the buffer and return it to pool
        uint32_t size = _alloc_next - _alloc_curr;
        memset((void*)_alloc_curr_ptr, 0, size);
        // Adjust start of next available block of the pool, allowing _alloc_prev to
        // have an invalid value (see next comment)
        _alloc_next = _alloc_curr;
        _alloc_curr = _alloc_prev;
        // Setting _alloc_prev_ptr=NULL guards against double free
        _alloc_curr_ptr = _alloc_prev_ptr;
        _alloc_prev_ptr = NULL;
#ifndef DBG_HIDE_ALLOC_FREE
        dbg("(OK pool[");
#endif
    } else {
        // Otherwise, allow buffer to leak
#ifndef DBG_HIDE_ALLOC_FREE
        dbg("-> (OK *LEAK* pool[");
#endif
    }
#ifndef DBG_HIDE_ALLOC_FREE
    dbg_u32(_alloc_next);
    dbg("]) ");
    dbg_alloc_stack(_alloc_curr_ptr, _alloc_prev_ptr);
    dbg("\n");
#endif
    return SL_STATUS_OK;
}

// Transmit frame
sl_status_t sl_wfx_host_transmit_frame(void *frame, uint32_t frame_len) {
#ifndef DBG_HIDE_SPI_TRANSFER
    dbg("(txfr ");
    dbg_hex32(frame);
    dbg(" ");
    dbg_u32(frame_len);
    dbg(")...\n "); // End debug line so it doesn't mix with sl_wfx_data_write
#endif
    sl_status_t status;
    status = sl_wfx_data_write(frame, frame_len);
#ifndef DBG_HIDE_SPI_TRANSFER
    if(status == SL_STATUS_OK) {
        dbg(" ...txfr -> OK\n");
    } else {
        dbg(" ...txfr -> FAIL(");
        dbg_hex32(status);
        dbg(")\n");
    }
#endif
    return status;
}

sl_status_t sl_wfx_host_lock(void) {
    return hal_lock();
}

sl_status_t sl_wfx_host_unlock(void) {
    return hal_unlock();
}

sl_status_t sl_wfx_host_init_bus(void) {
    dbg("init_bus\n");
    return hal_init_bus();
}

sl_status_t sl_wfx_host_deinit_bus(void) {
    dbg("deinit_bus() -> OK(_alloc_highwater_mark: ");
    dbg_u32(_alloc_highwater_mark);
    dbg(", context->used_buffers: ");
    if(_context_ptr == NULL) {
        dbg("NULL");
    } else {
        dbg_u16(_context_ptr->used_buffers);
    }
    dbg(")\n");
    return hal_deinit_bus();
}

sl_status_t sl_wfx_host_enable_platform_interrupt(void) {
    return hal_enable_interrupt();
}

sl_status_t sl_wfx_host_disable_platform_interrupt(void) {
    return hal_disable_interrupt();
}

sl_status_t sl_wfx_host_spi_cs_assert(void) {
    return hal_cs_assert();
}

sl_status_t sl_wfx_host_spi_cs_deassert(void) {
    return hal_cs_deassert();
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
    uint16_t buffer_length
) {
    // State variables for keeping in sync with wfx driver
    static uint32_t sram_base_address = 0;
    static uint32_t ncp_status_tick = 0;
    static uint32_t ctrl_area_put = 0;
    static bool download_complete = false;

#ifndef DBG_HIDE_SPI_TRANSFER
    // Debug print the raw byte array arguments
    dbg("(spi ");
    for(int i=0; i < header_length; i++) {
        dbg_hex8(header[i]);
    }
    if(buffer_length <= 8) {
        for(int i=0; i < buffer_length; i++) {
            dbg_hex8(buffer[i]);
        }
    } else {
        dbg(" <");
        dbg_u32(buffer_length);
        dbg(" bytes>");
    }
    dbg(") ");
#endif

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
#ifndef DBG_HIDE_SPI_TRANSFER
        dbg("(FAIL unsupported_transfer_type)\n");
#endif
        return SL_STATUS_FAIL;
    }
    sl_status_t result;
    result = hal_spi_transfer(read, header, header_length, buffer, buffer_length);

#ifndef DBG_HIDE_SPI_TRANSFER
    // All transfers should start with a 16-bit command word (the header), and
    // they often have a transfer length of two 16-bit words, (buffer_length ==
    // 4). The order in which bytes get arranged in the data buffer varries
    // based on the word mode.
    //
    // This attempts to interpret what was transferred.
    if(result != SL_STATUS_OK) {
        dbg("(FAIL ");
        dbg_hex32((uint32_t)result);
        dbg(")\n");
    } else if(header_length == 2 && buffer_length == 4) {
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
        if(cmd_rw == 'W') {
            dbg("(w ");
            dbg_decode_addr(cmd_addr);
            dbg(" ");
            dbg_decode_u32(buf); // this is the NON-packed buffer value
            dbg(") OK\n");
        } else {
            dbg("(r ");
            dbg_decode_addr(cmd_addr);
            dbg(") (OK ");
            dbg_decode_u32(buf);  // this is the NON-packed buffer value
            dbg(")\n");
        }
    } else {
        // Since (header_length == 2 && buffer_length == 4) was false, this is
        // not a ctrl/config register access. So, use simpler debug.
        dbg("OK\n");
    }
#endif // DBG_HIDE_SPI_TRANSFER
    return result;
}

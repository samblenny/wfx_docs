#include <stdint.h>
#include <stdbool.h>
#include "sl_wfx.h"
#include "dbg.h"

#ifdef SL_WFX_DEBUG_MASK
#include <stdarg.h> // for sl_wfx_host_log va_* stuff

// Pass sl_wfx debug logging straight to vprintf
void sl_wfx_host_log(const char *string, ...)
{
    va_list ap;
    va_start(ap, string);
    vprintf(string, ap);
    va_end(ap);
}
#endif

sl_status_t sl_wfx_host_init(void)
{
    dbg("init\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_get_firmware_data(
    const uint8_t **data,
    uint32_t data_size)
{
    dbg("get_firmware\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_firmware_size(uint32_t *firmware_size)
{
    dbg("get_firmware_size\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_pds_data(const char **pds_data, uint16_t index)
{
    dbg("get_pds_data\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_pds_size(uint16_t *pds_size)
{
    dbg("get_pds_size\n");
    return SL_STATUS_FAIL;
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
    dbg("setup_waited_event\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_wait_for_confirmation(
    uint8_t confirmation_id,
    uint32_t timeout_ms,
    void **event_payload_out)
{
    dbg("wait_for_confirmation\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_wait(uint32_t wait_ms)
{
    dbg("wait ");
    dbg_u32(wait_ms);
    dbg("ms; ");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_post_event(sl_wfx_generic_message_t *event_payload)
{
    dbg("post_event\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_allocate_buffer(
    void **buffer,
    sl_wfx_buffer_type_t type,
    uint32_t buffer_size)
{
    dbg("allocate_buffer\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_free_buffer(void *buffer, sl_wfx_buffer_type_t type)
{
    dbg("free_buffer\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_transmit_frame(void *frame, uint32_t frame_len)
{
    dbg("transmit_frame\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_lock(void)
{
    dbg("lock\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_unlock(void)
{
    dbg("unlock\n");
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
    dbg("cs=on; ");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_deassert(void)
{
    dbg("cs=off\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_transfer_no_cs_assert(
    sl_wfx_host_bus_transfer_type_t type,
    uint8_t *header,
    uint16_t header_length,
    uint8_t *buffer,
    uint16_t buffer_length)
{
    // Debug print the raw byte array arguments
    dbg("spi_transfer 0x");
    for(int i=0; i < header_length; i++) {
        dbg_hex8(header[i]);
    }
    dbg(" 0x");
    for(int i=0; i < buffer_length; i++) {
        dbg_hex8(buffer[i]);
    }
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
        uint16_t cmd_len = cmd & 0x0fff;
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
        dbg(" -> ");
        dbg_c(cmd_rw);
        dbg("(addr:");
        dbg_u16(cmd_addr);
        dbg(",len:");
        dbg_u16(cmd_len);
        dbg(",oBuf:0x");
        dbg_hex32(buf);
        dbg(")");
        bool addr_is_config_reg = cmd_addr == SL_WFX_CONFIG_REG_ID;
        bool addr_is_control_reg = cmd_addr == SL_WFX_CONTROL_REG_ID;
        if(addr_is_control_reg) {
            // When sl_wfx_init_chip() writes control with WUP bit set, it
            // expects to see responses with RDY set, or else it goes into a
            // big polling loop. So, claim to be always RDY.
            buf |= SL_WFX_CONT_RDY_BIT;
            // Re-pack buffer (Mode2) with the output value of buf
            buffer[3] = (uint8_t)(buf>>24);
            buffer[2] = (uint8_t)((buf<<8)>>24);
            buffer[1] = (uint8_t)((buf<<16)>>24);
            buffer[0] = (uint8_t)((buf<<24)>>24);
        } else if(addr_is_config_reg) {
            // This needs to be something other than 0x00000000 or 0xFFFFFFFF,
            // or else sl_wfx_init_bus() will abort with SL_STATUS_FAIL
            buf = 1;
            // Re-pack buffer (config is always Mode0) with output value of buf
            buffer[2] = (uint8_t)(buf>>24);
            buffer[3] = (uint8_t)((buf<<8)>>24);
            buffer[0] = (uint8_t)((buf<<16)>>24);
            buffer[1] = (uint8_t)((buf<<24)>>24);
        }
        // Debug print the packed output value of buffer
        dbg(" iBuf:0x");
        for(int i=0; i < buffer_length; i++) {
            dbg_hex8(buffer[i]);
        }
    }
    dbg("; ");
    return SL_STATUS_OK;
}

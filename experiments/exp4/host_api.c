#include <stdio.h>
#include "sl_wfx.h"

sl_status_t sl_wfx_host_init(void)
{
    printf("init\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_get_firmware_data(
    const uint8_t **data,
    uint32_t data_size)
{
    printf("get_firmware\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_firmware_size(uint32_t *firmware_size)
{
    printf("get_firmware_size\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_pds_data(const char **pds_data, uint16_t index)
{
    printf("get_pds_data\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_get_pds_size(uint16_t *pds_size)
{
    printf("get_pds_size\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_deinit(void)
{
    printf("deinit\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_reset_chip(void)
{
    printf("reset_chip\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_set_wake_up_pin(uint8_t state)
{
    printf("set_wake_up_pin\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_wait_for_wake_up(void)
{
    printf("wait_for_wake_up\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_sleep_grant(
    sl_wfx_host_bus_transfer_type_t type,
    sl_wfx_register_address_t address,
    uint32_t length)
{
    printf("sleep_grant\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_setup_waited_event(uint8_t event_id)
{
    printf("setup_waited_event\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_wait_for_confirmation(
    uint8_t confirmation_id,
    uint32_t timeout_ms,
    void **event_payload_out)
{
    printf("wait_for_confirmation\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_wait(uint32_t wait_ms)
{
    printf("wait %ums\n", wait_ms);
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_post_event(sl_wfx_generic_message_t *event_payload)
{
    printf("post_event\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_allocate_buffer(
    void **buffer,
    sl_wfx_buffer_type_t type,
    uint32_t buffer_size)
{
    printf("allocate_buffer\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_free_buffer(void *buffer, sl_wfx_buffer_type_t type)
{
    printf("free_buffer\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_transmit_frame(void *frame, uint32_t frame_len)
{
    printf("transmit_frame\n");
    return SL_STATUS_FAIL;
}

sl_status_t sl_wfx_host_lock(void)
{
    printf("lock\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_unlock(void)
{
    printf("unlock\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_init_bus(void)
{
    printf("init_bus\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_deinit_bus(void)
{
    printf("deinit_bus\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_enable_platform_interrupt(void)
{
    printf("enable_platform_interrupt\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_disable_platform_interrupt(void)
{
    printf("disable_platform_interrupt\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_assert(void)
{
    printf("spi_cs_assert\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_cs_deassert(void)
{
    printf("spi_cs_deassert\n");
    return SL_STATUS_OK;
}

sl_status_t sl_wfx_host_spi_transfer_no_cs_assert(
    sl_wfx_host_bus_transfer_type_t type,
    uint8_t *header,
    uint16_t header_length,
    uint8_t *buffer,
    uint16_t buffer_length)
{
    printf("spi_transfer_no_cs_assert 0x");
    for(int i=0; i < header_length; i++) {
        printf("%02X", header[i]);
    }
    printf(" 0x");
    for(int i=0; i < buffer_length; i++) {
        printf("%02X", buffer[i]);
    }
    printf("\n");
    return SL_STATUS_FAIL;
}


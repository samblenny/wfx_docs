#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C"
{
#endif

sl_status_t hal_init();
sl_status_t hal_deinit();
sl_status_t hal_init_bus();
sl_status_t hal_deinit_bus();
sl_status_t hal_reset_chip();
sl_status_t hal_lock();
sl_status_t hal_unlock();
sl_status_t hal_enable_interrupt();
sl_status_t hal_disable_interrupt();
sl_status_t hal_set_wup(uint8_t state);
sl_status_t hal_cs_assert();
sl_status_t hal_cs_deassert();
sl_status_t hal_wait(uint32_t wait_ms);

sl_status_t hal_spi_transfer(
    bool read,
    uint8_t *header, uint32_t header_len,
    uint8_t *buffer, uint32_t buffer_len
);

bool hal_wirq_asserted();

#ifdef __cplusplus
}
#endif

#endif // HAL_H

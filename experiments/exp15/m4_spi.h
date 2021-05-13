#ifndef M4_SPI_H
#define M4_SPI_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define M4_PIN_RESET A2
#define M4_PIN_WUP A3
#define M4_PIN_WIRQ A4
#define M4_PIN_CS A5
#define M4_PIN_FAKE_CS 12
#define M4_PIN_SCK SCK
#define M4_PIN_MOSI MOSI
#define M4_PIN_MISO MISO
#define M4_SPI_CLOCK_HZ 5000000

sl_status_t m4_init();
sl_status_t m4_deinit();
sl_status_t m4_init_bus();
sl_status_t m4_deinit_bus();
sl_status_t m4_reset_chip();
void m4_set_wup(uint8_t state);
void m4_wait(uint32_t wait_ms);
void m4_enable_interrupt();
void m4_disable_interrupt();
void m4_cs_assert();
void m4_cs_deassert();
sl_status_t m4_spi_transfer(
    bool read,
    uint8_t *header, uint32_t header_len,
    uint8_t *buffer, uint32_t buffer_len
);
bool m4_wirq_asserted();

#ifdef __cplusplus
}
#endif

#endif // M4_SPI_H

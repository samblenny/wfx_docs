#ifndef M4_SPI_H
#define M4_SPI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void m4_init();
void m4_deinit();
void m4_init_bus();
void m4_deinit_bus();
void m4_reset_chip();
void m4_set_wup(uint8_t state);
void m4_wait(uint32_t wait_ms);
void m4_tx_frame(void *frame, uint32_t frame_len);
void m4_enable_interrupt();
void m4_disable_interrupt();
void m4_cs_assert();
void m4_cs_deassert();

#ifdef __cplusplus
}
#endif

#endif // M4_SPI_H

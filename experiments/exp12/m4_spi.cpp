#include "m4_spi.h"
#include "dbg.h"

#ifdef NOT_ARDUINO
/*
 * Do lots of NOP for non-Arduino IDE builds
 */

void m4_init() {}
void m4_deinit() {}
void m4_init_bus() {}
void m4_deinit_bus() {}
void m4_reset_chip() {}
void m4_set_wup(uint8_t state) {}
void m4_wait(uint32_t wait_ms) {}
void m4_tx_frame(void *frame, uint32_t frame_len) {}
void m4_enable_interrupt() {}
void m4_disable_interrupt() {}
void m4_cs_assert() {}
void m4_cs_deassert() {}
void m4_spi_transfer(uint8_t *buffer, uint32_t buffer_len) {}

#else

/*
 * For Arduino IDE, do SPI over GPIO pins
 *
 *   Adafruit     Silicon Laboratories
 * Feather M4 <-> BRD8022A EXP header
 * ----------     --------------------
 *        3V3     2  (VMCU, wire through INA190 shunt)
 *        GND     1  (GND)
 *         A2     7  (RESETn)
 *         A3     3  (GPIO_WUP)
 *         A4     12 (SPI_WIRQ)
 *         A5     10 (SPI_CS)
 *        SCK     8  (SPI_SCLK)
 *       MOSI     4  (SPI_MOSI)
 *       MISO     6  (SPI_MISO)
 */
#include "Arduino.h"

void m4_init() {
    dbg("m4_init: A2:reset_hiZ A3:WUP_hiZ A4:WIRQ_in\n");
}

void m4_deinit() {
    dbg("m4_deinit: hiZ: A2 A3 A4 A5\n");
}

void m4_init_bus() {
    dbg("m4_init_bus: A5:CS_oHi hw_SPI(SCK MOSI MISO)\n");
}

void m4_deinit_bus() {
    dbg("m4_deinit_bus: A5:CS_hiZ hw_SPI_off(SCK MOSI MISO)\n");
}

void m4_reset_chip() {
    dbg("m4_reset_chip: A2:reset_oLo wait(1ms) A2:reset_hiZ\n");
}

void m4_set_wup(uint8_t state) {
    dbg("m4_set_wup: A3:WUP=(state>0?oHi:oLo)\n");
}

void m4_wait(uint32_t wait_ms) {
    dbg("m4_wait: ");
    dbg_u32(wait_ms);
    dbg("ms\n");
}

void m4_tx_frame(void *frame, uint32_t frame_len) {
    dbg("m4_tx_frame: hw_SPI.?()\n");
}

void m4_enable_interrupt() {
    dbg("m4_enable_interrupt: nop?\n");
}

void m4_disable_interrupt() {
    dbg("m4_disable_interrupt: nop?\n");
}

void m4_cs_assert() {
    dbg("m4_cs_assert: A5:CS_oLo\n");
}

void m4_cs_deassert() {
    dbg("m4_cs_deassert: A5:CS_hiZ\n");
}

void m4_spi_transfer(uint8_t *buffer, uint32_t buffer_len) {
    dbg("m4_spi_transfer: buffer:... buffer_len:");
    dbg_u32(buffer_len);
    dbg("\n");
}

#endif // NOT_ARDUINO

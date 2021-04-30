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
void m4_spi_transfer(
    bool read,
    uint8_t *header, uint32_t header_len,
    uint8_t *buffer, uint32_t buffer_len
) {}

#else

/*
 * For Arduino IDE, do SPI over GPIO pins
 *
 *     Adafruit     Silicon Laboratories
 *   Feather M4 <-> BRD8022A EXP header
 * ------------     --------------------
 *          3V3     2  (VMCU, wire through INA190 shunt)
 *          GND     1  (GND)
 *           A2     7  (RESETn)
 *           A3     3  (GPIO_WUP)
 *           A4     12 (SPI_WIRQ)
 *           A5     10 (SPI_CS)
 *          SCK     8  (SPI_SCLK)
 *         MOSI     4  (SPI_MOSI)
 *         MISO     6  (SPI_MISO)
 * (fake CS) 13     N/C (fake CS pin to trick the Arduino hardware SPI library)
 *
 * NOTE: The SPI code below uses Adafruit's busio library, so be sure to use the
 * Arduino IDE library manager (tools menu) to install Adafruit BusIO.
 * Related docs & source:
 * - https://www.arduino.cc/reference/en/libraries/adafruit-busio/
 * - https://github.com/adafruit/Adafruit_BusIO
 */
#include "Arduino.h"
#include <Adafruit_SPIDevice.h>

#define M4_PIN_RESET A2
#define M4_PIN_WUP A3
#define M4_PIN_WIRQ A4
#define M4_PIN_CS A5
#define M4_PIN_FAKE_CS 13
#define M4_PIN_SCK SCK
#define M4_PIN_MOSI MOSI
#define M4_PIN_MISO MISO

Adafruit_SPIDevice _af_spi = Adafruit_SPIDevice(M4_PIN_FAKE_CS);
bool _af_spi_valid = false;

void m4_init() {
    dbg("m4_init: A2:reset_hiZ A3:WUP_hiZ A4:WIRQ_in\n");
    pinMode(M4_PIN_RESET, INPUT);
    pinMode(M4_PIN_WUP, INPUT);
    pinMode(M4_PIN_WIRQ, INPUT);
}

void m4_deinit() {
    dbg("m4_deinit: hiZ: A2:RESET A3:WUP A4:WIRQ\n");
    pinMode(M4_PIN_RESET, INPUT);
    pinMode(M4_PIN_WUP, INPUT);
    pinMode(M4_PIN_WIRQ, INPUT);
}

void m4_init_bus() {
    dbg("m4_init_bus: A5:CS_oHi hw_SPI(SCK MOSI MISO)\n");
    digitalWrite(M4_PIN_CS, HIGH); // 1. CS: hiZ -> pullup
    pinMode(M4_PIN_CS, OUTPUT);    // 2. CS: pullup -> drive high
    if(!_af_spi.begin()) {
        dbg("m4_init_bus: _af_spi.begin() FAILED (this is probably a bug)\n");
        _af_spi_valid = false;
    } else {
        dbg("m4_init_bus: _af_spi.begin() -> OK\n");
        _af_spi_valid = true;
    }
}

void m4_deinit_bus() {
    dbg("m4_deinit_bus: A5:CS_hiZ hw_SPI_off(SCK MOSI MISO)\n");
    _af_spi_valid = false;
    pinMode(M4_PIN_CS, INPUT);
    pinMode(M4_PIN_SCK, INPUT);
    pinMode(M4_PIN_MOSI, INPUT);
    pinMode(M4_PIN_MISO, INPUT);
    pinMode(M4_PIN_FAKE_CS, INPUT);
}

void m4_reset_chip() {
    dbg("m4_reset_chip: A2:reset_oLo wait(1ms) A2:reset_hiZ\n");
    digitalWrite(M4_PIN_RESET, LOW);
    pinMode(M4_PIN_RESET, OUTPUT);
    delay(1);
    pinMode(M4_PIN_RESET, INPUT);
    digitalWrite(M4_PIN_RESET, HIGH);
}

void m4_set_wup(uint8_t state) {
    dbg("m4_set_wup: A3:WUP=(state>0?oHi:oLo)\n");
    digitalWrite(M4_PIN_WUP, state>0 ? HIGH : LOW);
    pinMode(M4_PIN_WUP, OUTPUT);
}

void m4_wait(uint32_t wait_ms) {
    dbg("m4_wait: ");
    dbg_u32(wait_ms);
    dbg("ms\n");
    delay(wait_ms);
}

void m4_tx_frame(void *frame, uint32_t frame_len) {
    dbg("m4_tx_frame: frame_len:");
    dbg_u32(frame_len);
    if(!_af_spi_valid) {
        dbg(" -> FAIL(!_af_spi_valid)\n");
        return;
    }
    dbg("\n");
    _af_spi.write((uint8_t *)frame, frame_len);
}

void m4_enable_interrupt() {
    dbg("m4_enable_interrupt: nop?\n");
}

void m4_disable_interrupt() {
    dbg("m4_disable_interrupt: nop?\n");
}

void m4_cs_assert() {
    dbg("m4_cs_assert: A5:CS_oLo\n");
    digitalWrite(M4_PIN_CS, LOW);
}

void m4_cs_deassert() {
    dbg("m4_cs_deassert: A5:CS_hiZ\n");
    digitalWrite(M4_PIN_CS, HIGH);
}

void m4_spi_transfer(
    bool read,
    uint8_t *header, uint32_t header_len,
    uint8_t *buffer, uint32_t buffer_len
) {
    dbg("m4_spi_transfer: read:");
    dbg(read ? "T" : "F");
    dbg(" header_len:");
    dbg_u32(header_len);
    dbg(" buffer_len:");
    dbg_u32(buffer_len);
    if(!_af_spi_valid) {
        dbg(" -> FAIL(!_af_spi_valid)\n");
        return;
    }
    dbg("\n");
    const uint8_t send_val = 0;
    // Send the 16-bit header (fake CS pin + m4_cs_(de)assert funcs for framing)
    _af_spi.write(header, header_len);
    // For read-type transfer, read. Otherwise, write
    if(read) {
        _af_spi.read((uint8_t*)buffer, buffer_len, send_val);
    } else {
        _af_spi.write((uint8_t*)buffer, buffer_len);
    }
}

#endif // NOT_ARDUINO

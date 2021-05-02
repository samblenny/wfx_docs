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
 * For Arduino IDE, do SPI over GPIO pins in test circuit like so:
 *
 *     Adafruit   Saleae   Silicon Laboratories   INA190EVM
 *   Feather M4   Logic8   BRD8022A EXP header    current sense
 * ------------   ------   --------------------   -------------
 *          GND     GND    1  GND                 GND
 *           A2   0 blk    7  RESETn              --
 *           A3   1 brn    3  GPIO_WUP            --
 *           A4   2 red    12 SPI_WIRQ            --
 *           A5   3 org    10 SPI_CS              --
 *          SCK   4 yel    8  SPI_SCLK            --
 *         MOSI   5 grn    4  SPI_MOSI            --
 *         MISO   6 blu    6  SPI_MISO            --
 * (fake CS) 13     --     --                     --
 *    (5V)  USB     --     --                     VS  (INA190 Vs)
 *          3V3     --     --                     IN+ (0.130Ω shunt)
 *           --   7 vlt    2  VMCU                IN- (0.130Ω shunt)
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

#define SPI_CLOCK_HZ 5000000
Adafruit_SPIDevice _af_spi = Adafruit_SPIDevice(M4_PIN_FAKE_CS, SPI_CLOCK_HZ);
bool _af_spi_valid = false;

void m4_init() {
    dbg("m4_init\n");
    pinMode(M4_PIN_RESET, INPUT);
    pinMode(M4_PIN_WUP, INPUT);
    pinMode(M4_PIN_WIRQ, INPUT);
}

void m4_deinit() {
    dbg("m4_deinit\n");
    pinMode(M4_PIN_RESET, INPUT);
    pinMode(M4_PIN_WUP, INPUT);
    pinMode(M4_PIN_WIRQ, INPUT);
}

void m4_init_bus() {
    dbg("m4_init_bus -> ");
    digitalWrite(M4_PIN_CS, HIGH); // 1. CS: hiZ -> pullup
    pinMode(M4_PIN_CS, OUTPUT);    // 2. CS: pullup -> output
    digitalWrite(M4_PIN_CS, HIGH); // 3. CS: drive high
    if(!_af_spi.begin()) {
        dbg("FAIL\n");
        _af_spi_valid = false;
    } else {
        dbg("OK\n");
        _af_spi_valid = true;
    }
}

void m4_deinit_bus() {
    dbg("m4_deinit_bus\n");
    _af_spi_valid = false;
    pinMode(M4_PIN_CS, INPUT);
    pinMode(M4_PIN_SCK, INPUT);
    pinMode(M4_PIN_MOSI, INPUT);
    pinMode(M4_PIN_MISO, INPUT);
    pinMode(M4_PIN_FAKE_CS, INPUT);
}

void m4_reset_chip() {
    dbg("m4_reset_chip\n");
    digitalWrite(M4_PIN_RESET, LOW);
    pinMode(M4_PIN_RESET, OUTPUT);
    delay(1);
    pinMode(M4_PIN_RESET, INPUT);
    digitalWrite(M4_PIN_RESET, HIGH);
}

void m4_set_wup(uint8_t state) {
    dbg("m4_set_wup\n");
    digitalWrite(M4_PIN_WUP, state>0 ? HIGH : LOW);
    pinMode(M4_PIN_WUP, OUTPUT);
}

void m4_wait(uint32_t wait_ms) {
    dbg("m4_wait ");
    dbg_u32(wait_ms);
    dbg("ms\n");
    delay(wait_ms);
}

void m4_tx_frame(void *frame, uint32_t frame_len) {
    dbg("m4_tx_frame: len:");
    dbg_u32(frame_len);
    if(!_af_spi_valid) {
        dbg(" -> FAIL\n");
    } else {
        dbg(" -> OK\n");
        _af_spi.write((uint8_t *)frame, frame_len);
    }
}

void m4_enable_interrupt() {
    // TODO: implement this? maybe leave as nop?
    dbg("m4_enable_interrupt\n");
}

void m4_disable_interrupt() {
    // TODO: implement this? maybe leave as nop?
    dbg("m4_disable_interrupt\n");
}

void m4_cs_assert() {
    dbg("m4_cs_assert\n");
    digitalWrite(M4_PIN_CS, LOW);
}

void m4_cs_deassert() {
    dbg("m4_cs_deassert\n");
    digitalWrite(M4_PIN_CS, HIGH);
}

void m4_spi_transfer(
    bool read,
    uint8_t *header, uint32_t header_len,
    uint8_t *buffer, uint32_t buffer_len
) {
    dbg("m4_spi_transfer: ");
    dbg(read ? "R" : "W");
    dbg(" h_len:");
    dbg_u32(header_len);
    dbg(" b_len:");
    dbg_u32(buffer_len);
    if(!_af_spi_valid) {
        dbg(" -> FAIL\n");
    } else {
        const uint8_t send_val = 0;
        _af_spi.write(header, header_len);
        if(read) {
            _af_spi.read((uint8_t*)buffer, buffer_len, send_val);
        } else {
            _af_spi.write((uint8_t*)buffer, buffer_len);
        }
        dbg(" -> OK\n");
    }
}

#endif // NOT_ARDUINO

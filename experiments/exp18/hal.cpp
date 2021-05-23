#include "hal.h"
#include "dbg.h"
#include <typeinfo>

#ifndef DO_NOT_USE_ARDUINO_API
/*
 * This ifndef block implements an SPI PHY interface using Arduino APIs. For
 * use on another platform, define DO_NOT_USE_ARDUINO_API and link against your
 * own implementations.
 */
#include "Arduino.h"
#include <Adafruit_SPIDevice.h>

#define M4_PIN_RESET A2
#define M4_PIN_WUP A3
#define M4_PIN_WIRQ A4
#define M4_PIN_CS A5
#define M4_PIN_FAKE_CS 12
#define M4_PIN_SCK SCK
#define M4_PIN_MOSI MOSI
#define M4_PIN_MISO MISO
#define M4_SPI_CLOCK_HZ 5000000

/*
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
 * (fake CS) 12     --     --                     --
 *    (5V)  USB     --     --                     VS  (INA190 Vs)
 *          3V3     --     20 3V3                 IN+ (0.130Ω shunt)
 *           --     --     2  VMCU                IN- (0.130Ω shunt)
 *           --   7 vlt    --                     OUT
 *
 * NOTE: The SPI code below uses Adafruit's busio library, so be sure to use the
 * Arduino IDE library manager (tools menu) to install Adafruit BusIO.
 * Related docs & source:
 * - https://www.arduino.cc/reference/en/libraries/adafruit-busio/
 * - https://github.com/adafruit/Adafruit_BusIO
 */

Adafruit_SPIDevice *_af_spi = NULL;
bool _af_spi_valid = false;

sl_status_t hal_init() {
    dbg("hal_init\n");
    pinMode(M4_PIN_RESET, INPUT);
    pinMode(M4_PIN_WUP, INPUT);
    pinMode(M4_PIN_WIRQ, INPUT);
    return SL_STATUS_OK;
}

sl_status_t hal_deinit() {
    dbg("hal_deinit\n");
    pinMode(M4_PIN_RESET, INPUT);
    pinMode(M4_PIN_WUP, INPUT);
    pinMode(M4_PIN_WIRQ, INPUT);
    return SL_STATUS_OK;
}

sl_status_t hal_init_bus() {
    if(_af_spi == NULL) {
        _af_spi = new Adafruit_SPIDevice(M4_PIN_FAKE_CS, M4_SPI_CLOCK_HZ);
    }
    pinMode(M4_PIN_CS, OUTPUT);
    digitalWrite(M4_PIN_CS, HIGH);
    if(!_af_spi->begin()) {
        dbg("hal_deinit_bus -> FAIL\n");
        _af_spi_valid = false;
        return SL_STATUS_FAIL;
    } else {
        _af_spi_valid = true;
        return SL_STATUS_OK;
    }
}

sl_status_t hal_deinit_bus() {
    dbg("hal_deinit_bus\n");
    _af_spi_valid = false;
    pinMode(M4_PIN_CS, INPUT_PULLUP);
    pinMode(M4_PIN_SCK, INPUT);
    pinMode(M4_PIN_MOSI, INPUT);
    pinMode(M4_PIN_MISO, INPUT);
    pinMode(M4_PIN_FAKE_CS, INPUT);
    hal_deinit();
    return SL_STATUS_OK;
}

sl_status_t hal_reset_chip() {
    pinMode(M4_PIN_RESET, OUTPUT);
    digitalWrite(M4_PIN_RESET, LOW);
    delay(10);
    digitalWrite(M4_PIN_RESET, HIGH);
    delay(10);
    pinMode(M4_PIN_RESET, INPUT);
    return SL_STATUS_OK;
}

sl_status_t hal_set_wup(uint8_t state) {
    digitalWrite(M4_PIN_WUP, state > 0 ? HIGH : LOW);
    return SL_STATUS_OK;
}

sl_status_t hal_wait(uint32_t wait_ms) {
    delay(wait_ms);
    return SL_STATUS_OK;
}

sl_status_t hal_lock()   { return SL_STATUS_OK; /* NOP */ }
sl_status_t hal_unlock() { return SL_STATUS_OK; /* NOP */ }

sl_status_t hal_enable_interrupt()  { return SL_STATUS_OK; /* NOP */ }
sl_status_t hal_disable_interrupt() { return SL_STATUS_OK; /* NOP */ }

sl_status_t hal_cs_assert() {
    digitalWrite(M4_PIN_CS, LOW);
    return SL_STATUS_OK;
}

sl_status_t hal_cs_deassert() {
    digitalWrite(M4_PIN_CS, HIGH);
    return SL_STATUS_OK;
}

sl_status_t hal_spi_transfer(
    bool read,
    uint8_t *header, uint32_t header_len,
    uint8_t *buffer, uint32_t buffer_len
) {
    if(!_af_spi_valid) {
        return SL_STATUS_FAIL;
    } else {
        const uint8_t send_val = 0;
        _af_spi->write(header, header_len);
        if(read) {
            _af_spi->read((uint8_t*)buffer, buffer_len, send_val);
        } else {
            _af_spi->write((uint8_t*)buffer, buffer_len);
        }
        return SL_STATUS_OK;
    }
}

// Return true if WF200 is asserting the WIRQ line
bool hal_wirq_asserted() {
    return digitalRead(M4_PIN_WIRQ) == HIGH;
}

#endif // ifndef DO_NOT_USE_ARDUINO_API

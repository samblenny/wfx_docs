/**
 * Method to use wfx-fullMAC-driver as an Arduino library:
 *    cd ~/Arduino
 *    git clone https://github.com/samblenny/wfx-fullMAC-driver.git --branch zero-buf
 *    cd libraries;       ln -s ../wfx-fullMAC-driver/wfx_fmac_driver wfx_fmac_driver
 *    cd wfx_fmac_driver; ln -s bus/sl_wfx_bus.c sl_wfx_bus.c
 *    ln -s bus/sl_wfx_bus.h sl_wfx_bus.h; ln -s bus/sl_wfx_bus_spi.c sl_wfx_bus_spi.c
 *
 * This works around limitations in how the Arduino IDE compiles libraries.
 */

#include "sl_wfx.h"

extern "C" {
  sl_wfx_context_t context;
}

void setup() {
  sl_status_t status;
  Serial.begin(57600);
  while (!Serial) {;}
  status = sl_wfx_init(&context);
}

void loop() {
}

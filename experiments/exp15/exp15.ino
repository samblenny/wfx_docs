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
#include "dbg.h"
#include "m4_spi.h"

extern "C" void remember_context_ptr(sl_wfx_context_t *);

extern "C" {
  sl_wfx_context_t context;
}

void setup() {
  sl_status_t status;
  pinMode(M4_PIN_RESET, INPUT_PULLUP);
  // Wait up to 3000ms for a USB serial monitor to connect
  // (USB connect delays in range of 1100..2300ms are typical)
  Serial.begin(115200);
  bool allow_serial = true;
  int i = 0;
  for(i=0; i<300; i++) {
    if(!Serial) {
      delay(10);
    } else {
      allow_serial = true;
      break;
    }
  }
  dbg_serial_ok(allow_serial);
  dbg_set_mute(true);
  // Initialize the radio
  remember_context_ptr(&context);
  status = sl_wfx_init(&context);
  if(status == SL_STATUS_OK) {
    dbg("============================================\n");
    dbg("sl_wfx_init() -> OK, calling sl_wfx_deinit()\n");
    dbg("============================================\n");
    sl_wfx_deinit();
  }
}

void loop() {
}

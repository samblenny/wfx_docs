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
#include "hal.h"
#include "state.h"

extern "C" void remember_context_ptr(sl_wfx_context_t *);

extern "C" {
  sl_wfx_context_t context;
}

// Attempt to read pending frames
sl_status_t drain_pending_frames(uint16_t limit) {
  uint16_t ctrl_reg = 0;
  sl_status_t result;
  for(uint16_t i=0; i<limit; i++) {
    result = sl_wfx_receive_frame(&ctrl_reg);
    if(result != SL_STATUS_OK) {
      break;
    }
    dbg((i%80!=79)?"$":"$\n");
  }
  return result;
}

void setup() {
  sl_status_t result;
  hal_init();
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
  result = sl_wfx_init(&context);
  if(result == SL_STATUS_OK) {
    dbg("============================\n");
    dbg("=== sl_wfx_init() -> OK ====\n");
    dbg("============================\n");
  } else {
    return;
  }
  dbg_set_mute(false);
  uint16_t ctrl_reg = 0;
  // Start SSID Scan
  drain_pending_frames(500);
  hal_wait(200);
  dbg("### Starting Active SSID Scan ###\n");
  sl_wfx_send_scan_command(WFM_SCAN_MODE_ACTIVE, NULL, 0, NULL, 0, NULL, 0, NULL);
  // Wait for scan results
  for(int i=0; state_ssid_scanning() && i<500; i++) {
    if(!hal_wirq_asserted()) {
      hal_wait(3);
      dbg((i%80!=79) ? "." : ".\n");
    }
    result = drain_pending_frames(500);
  }
  hal_wait(200);
  dbg("\n### Stoping Active SSID Scan ###\n");
  sl_wfx_send_stop_scan_command();
  hal_wait(200);
  dbg("\n### Starting Passive SSID Scan ###\n");
  sl_wfx_send_scan_command(WFM_SCAN_MODE_PASSIVE, NULL, 0, NULL, 0, NULL, 0, NULL);
  // Wait for scan results
  for(int i=0; state_ssid_scanning() && i<500; i++) {
    if(!hal_wirq_asserted()) {
      hal_wait(3);
      dbg((i%80!=79) ? "." : ".\n");
    }
    result = drain_pending_frames(500);
  }
  hal_wait(200);
  dbg("\n### Stoping Passive SSID Scan ###\n");
  sl_wfx_send_stop_scan_command();
  hal_wait(200);
  dbg("_\n");
  sl_wfx_deinit();
}

void loop() {
}

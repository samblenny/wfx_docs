#include "dbg.h"
#include "state.h"

static bool SSID_SCANNING = false;

void state_set_ssid_scanning(bool currently_scanning) {
    SSID_SCANNING = currently_scanning;
    dbg("(ssid_scanning ");
    dbg(currently_scanning ? "true)\n" : "false)\n");
}

bool state_ssid_scanning() {
    return SSID_SCANNING;
}

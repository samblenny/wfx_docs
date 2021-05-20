#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C"
{
#endif

void state_set_ssid_scanning(bool active);
bool state_ssid_scanning();

#ifdef __cplusplus
}
#endif

#endif // STATE_H

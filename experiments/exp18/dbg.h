/**
 * Debug helper functions
 */
#ifndef DBG_H
#define DBG_H

#include <stdint.h>
#include <stdbool.h>
#include "firmware/sl_wfx_general_api.h"
#include "sl_wfx_constants.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Change this to false to see MAC addresses from startup_ind_body_t
#define DBG_HIDE_MAC true

// Comment these out to see additional debug tracing
#define DBG_HIDE_ALLOC_FREE
#define DBG_HIDE_SPI_TRANSFER


void dbg_serial_ok(bool allow_serial);
void dbg_set_mute(bool mute);
void dbg(const char *str);
void dbg_i8(int8_t val);
void dbg_u8(uint8_t val);
void dbg_u16(uint16_t val);
void dbg_u32(uint32_t val);
void dbg_hex8(uint8_t val);
void dbg_hex16(uint16_t val);
void dbg_hex32(uint32_t val);
void dbg_decode_addr(uint16_t addr);
void dbg_decode_u32(uint32_t val);
void dbg_mac_addr(uint8_t *mac);
void dbg_startup_ind(sl_wfx_startup_ind_t *startup);
void dbg_buffer_type(sl_wfx_buffer_type_t type);
void dbg_alloc_stack(uint8_t *prev_ptr, uint8_t *curr_ptr);
void dbg_message_id(uint8_t id);
void dbg_scan_result(sl_wfx_scan_result_ind_body_t *sr);

#ifdef __cplusplus
}
#endif

#endif // DBG_H

/**
 * Debug helper functions
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dbg.h"
#include "firmware/sl_wfx_registers.h"

#ifndef DO_NOT_USE_ARDUINO_API
/*
 * This ifndef block implements debug string conversions and UART PHY using
 * Arduino APIs. For use on another platform, define DO_NOT_USE_ARDUINO_API
 * and link against your own implementations.
 */
#include "Arduino.h"

static bool DBG_SERIAL_OK = false;
static bool DBG_MUTE = true;

void dbg_serial_ok(bool allow_serial) { DBG_SERIAL_OK = allow_serial; }

void dbg_set_mute(bool mute) {
    // When serial port is disconnected, debug must stay muted. Otherwise, allow selection.
    DBG_MUTE = DBG_SERIAL_OK ? mute : true;
}

void dbg(const char *str)  { if(!DBG_MUTE) { Serial.print(str);      Serial.flush(); } }
void dbg_i8(int8_t val)    { if(!DBG_MUTE) { Serial.print(val, DEC); Serial.flush(); } }
void dbg_u8(uint8_t val)   { if(!DBG_MUTE) { Serial.print(val, DEC); Serial.flush(); } }
void dbg_u16(uint16_t val) { if(!DBG_MUTE) { Serial.print(val, DEC); Serial.flush(); } }
void dbg_u32(uint32_t val) { if(!DBG_MUTE) { Serial.print(val, DEC); Serial.flush(); } }

void dbg_hex8(uint8_t val) {
    if(DBG_MUTE) { return; }
    if (val < 0x10) { Serial.print("0"); }
    Serial.print(val, HEX);
    Serial.flush();
}

void dbg_hex16(uint16_t val) {
    if(DBG_MUTE) { return; }
    if      (val < 0x10)   { Serial.print("000"); }
    else if (val < 0x100)  { Serial.print("00"); }
    else if (val < 0x1000) { Serial.print("0"); }
    Serial.print(val, HEX);
    Serial.flush();
}

void dbg_hex32(uint32_t val) {
    if(DBG_MUTE) { return; }
    if      (val < 0x10)       { Serial.print("0000000"); }
    else if (val < 0x100)      { Serial.print("000000"); }
    else if (val < 0x1000)     { Serial.print("00000"); }
    else if (val < 0x10000)    { Serial.print("0000"); }
    else if (val < 0x100000)   { Serial.print("000"); }
    else if (val < 0x1000000)  { Serial.print("00"); }
    else if (val < 0x10000000) { Serial.print("0"); }
    else                       { /*Serial.print("");*/ }
    Serial.print(val, HEX);
    Serial.flush();
}
#endif // ifndef DO_NOT_USE_ARDUINO_API

/*
 * Decode address bits of SPI header.
 * These are useful for tracing wfx_fmac_driver control flow.
 */
void dbg_decode_addr(uint16_t addr) {
    switch(addr) {
    case 0: dbg("Conf"); break;
    case 1: dbg("Ctrl"); break;
    case 2: dbg("IO_Queue"); break;
    case 3: dbg("AHB_Data"); break;
    case 4: dbg("SRAM_Adr"); break;
    case 5: dbg("SRAM_Dat"); break;
    case 6: dbg("TSET_Gen"); break;
    case 7: dbg("FrameOut"); break;
    default:
        dbg_u16(addr);
    }
}

/*
 * Decode magic values for SRAM_BASE_ADDR_REG and SRAM_DPORT_REG.
 * These are useful for tracing progress during the firmware transfer.
 */
void dbg_decode_u32(uint32_t val) {
    switch(val) {
    case 0x09002000: dbg("SHARED_RAM_DEBUG_AREA"); break;
    case 0x09004000: dbg("DOWNLOAD_FIFO_BASE");    break;
    case ADDR_DWL_CTRL_AREA_IMAGE_SIZE:  dbg("CTRL_AREA_IMAGE_SIZE");  break;
    case ADDR_DWL_CTRL_AREA_PUT:         dbg("CTRL_AREA_PUT");         break;
    case ADDR_DWL_CTRL_AREA_GET:         dbg("CTRL_AREA_GET");         break;
    case ADDR_DWL_CTRL_AREA_HOST_STATUS: dbg("CTRL_AREA_HOST_STATUS"); break;
    case ADDR_DWL_CTRL_AREA_NCP_STATUS:  dbg("CTRL_AREA_NCP_STATUS");  break;
    case ADDR_DWL_CTRL_AREA_SIGNATURE:   dbg("CTRL_AREA_SIGNATURE");   break;
    case ADDR_DWL_CTRL_AREA_FW_HASH:     dbg("CTRL_AREA_FW_HASH");     break;
    case ADDR_DWL_CTRL_AREA_FW_VERSION:  dbg("CTRL_AREA_FW_VERSION");  break;
    case 0x12345678: dbg("*_STATE_NOT_READY");           break;
    case 0x7AB41D19: dbg("NCP_STATE_PUB_KEY_RDY");       break;
    case 0x87654321: dbg("*_STATE_READY");               break;
    case 0xA753BD99: dbg("HOST_STATE_HOST_INFO_READ");   break;
    case 0xABCDDCBA: dbg("*_STATE_*LOAD_PENDING");       break;
    case 0xBD53EF99: dbg("NCP_STATE_INFO_READY");        break;
    case 0xCAFEFECA: dbg("NCP_STATE_DOWNLOAD_COMPLETE"); break;
    case 0xD4C64A99: dbg("*_STATE_(*_COMPLETE|*_OK)");   break;
    case 0xFFFFFFFF: dbg("*_STATE_UNDEF");               break;
    case HOST_STATE_OK_TO_JUMP:
        dbg("HOST_STATE_OK_TO_JUMP");
        break;
    default:
        dbg_hex32(val);
    }
}

// Decode a MAC address of size SL_WFX_MAC_ADDR_SIZE
void dbg_mac_addr(uint8_t *mac) {
    for(int i=0; i<SL_WFX_MAC_ADDR_SIZE; i++) {
        if(DBG_HIDE_MAC) {
            dbg("--");
        } else {
            dbg_hex8(mac[i]);
        }
        if(i+1 < SL_WFX_MAC_ADDR_SIZE) {
            dbg(":");
        }
    }
}

// Dump the startup response (firmware version, MAC addresses, etc.)
void dbg_startup_ind(sl_wfx_startup_ind_t *startup) {
    dbg("startup_ind:");
    if(startup == NULL) {
        dbg(" -> FAIL(startup==NULL)\n");
        return;
    }
    sl_wfx_startup_ind_body_t *b = &(startup->body);
    dbg("\n  status: ");
    dbg_hex32(b->status);
    dbg("\n  hardware_id: ");
    dbg_hex16(b->hardware_id);
    dbg("\n  num_inp_ch_bufs: ");
    dbg_u16(b->num_inp_ch_bufs);
    dbg("\n  size_inp_ch_buf: ");
    dbg_u16(b->size_inp_ch_buf);
    for(uint8_t i=0; i<2; i++) {
        dbg("\n  mac_addr[");
        dbg_u8(i);
        dbg("]: ");
        dbg_mac_addr(b->mac_addr[i]);
    }
    dbg("\n  api_version_minor: ");
    dbg_u8(b->api_version_minor);
    dbg("\n  api_version_major: ");
    dbg_u8(b->api_version_major);
    dbg("\n  firmware_build: ");
    dbg_u8(b->firmware_build);
    dbg("\n  firmware_minor: ");
    dbg_u8(b->firmware_minor);
    dbg("\n  firmware_major: ");
    dbg_u8(b->firmware_major);
    dbg("\n  firmware_type: ");
    dbg_u8(b->firmware_type);
    dbg("\n");
}

// Decode buffer types used with sl_wfx_host_(allocate_buffer|free_buffer)
void dbg_buffer_type(sl_wfx_buffer_type_t type) {
    switch(type) {
    case SL_WFX_TX_FRAME_BUFFER:
        dbg("TXFR_BUF");
        break;
    case SL_WFX_RX_FRAME_BUFFER:
        dbg("RXFR_BUF");
        break;
    case SL_WFX_CONTROL_BUFFER:
        dbg("CTRL_BUF");
        break;
    }
}

// Dump the stack of recently allocated buffer pointers (like a free-list)
void dbg_alloc_stack(uint8_t *prev_ptr, uint8_t *curr_ptr) {
    dbg("(astk ");
    dbg_hex32((uint32_t)prev_ptr);
    dbg(" ");
    dbg_hex32((uint32_t)curr_ptr);
    dbg(")");
}

// Decode a event message id (e.g. SL_WFX_.*_ID)
void dbg_message_id(uint8_t id) {
    switch(id) {
    case SL_WFX_STARTUP_IND_ID:
        dbg("STARTUP_IND");
        break;
    case SL_WFX_CONFIGURATION_REQ_ID:
        dbg("CONFIGURATION_REQ");
        break;
    case SL_WFX_START_SCAN_CNF_ID:
        dbg("START_SCAN_CNF");
        break;
    case SL_WFX_STOP_SCAN_CNF_ID:
        dbg("STOP_SCAN_CNF");
        break;
    case SL_WFX_SCAN_RESULT_IND_ID:
        dbg("SCAN_RESULT");
        break;
    case SL_WFX_SCAN_COMPLETE_IND_ID:
        dbg("SCAN_COMPLETE");
        break;
    default:
        dbg_hex16(id);
    }
}

// Decode an SSID scan result message
void dbg_scan_result(sl_wfx_scan_result_ind_body_t *sr) {
    // Ignore hidden SSIDs (lenth 0, or null chars for non-zero length SSID)
    if(sr->ssid_def.ssid_length == 0 || sr->ssid_def.ssid[0] == 0) {
        return;
    }
    // Copy the SSID to a new array to make 100% sure it's null terminated
    char ssid[SL_WFX_SSID_SIZE+1];
    memset((void *)ssid, 0, SL_WFX_SSID_SIZE+1);
    uint32_t len = sr->ssid_def.ssid_length;
    len = (SL_WFX_SSID_SIZE < len) ? SL_WFX_SSID_SIZE : len;
    if(DBG_HIDE_MAC) {
        memset((void *)ssid, (uint8_t)'-', len);
    } else {
        memcpy((void *)ssid, (void *)(sr->ssid_def.ssid), len);
    }

    // Debug print the scan result
    dbg("(scan ");
    dbg_mac_addr(sr->mac);
    dbg(" ");
    dbg_i8((sr->rcpi/2) - 110);
    dbg("dBm ch");
    dbg_u8(sr->channel);
    dbg(" '");
    dbg((const char *)ssid);
    dbg("')\n");
}

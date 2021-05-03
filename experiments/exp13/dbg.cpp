/**
 * Debug helper functions for POSIX/Arduino portability
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dbg.h"
#include "firmware/sl_wfx_registers.h"

static bool DBG_SERIAL_OK = false;
static bool DBG_MUTE = true;

void dbg_serial_ok(bool allow_serial) { DBG_SERIAL_OK = allow_serial; }

#ifdef NOT_ARDUINO
/**
 * For building with make, send debug to stdout with printf.
 */
void dbg_set_mute(bool mute) { DBG_MUTE = mute; }
void dbg(const char *str)    { if(!DBG_MUTE) { printf("%s", str);     } }
void dbg_c(char c)           { if(!DBG_MUTE) { printf("%c", c);       } }
void dbg_u8(uint8_t val)     { if(!DBG_MUTE) { printf("%u", val);     } }
void dbg_u16(uint16_t val)   { if(!DBG_MUTE) { printf("%u", val);     } }
void dbg_u32(uint32_t val)   { if(!DBG_MUTE) { printf("%u", val);     } }
void dbg_hex8(uint8_t val)   { if(!DBG_MUTE) { printf("%02X", val);   } }
void dbg_hex16(uint16_t val) { if(!DBG_MUTE) { printf("%04X", val);   } }
void dbg_hex32(uint32_t val) { if(!DBG_MUTE) { printf("0x%08X", val); } }


#else
/**
 * For building with the Arduino IDE, send debug to serial port.
 */
#include "Arduino.h"

void dbg_set_mute(bool mute) {
    // When serial port is disconnected, debug must stay muted. Otherwise, allow selection.
    DBG_MUTE = DBG_SERIAL_OK ? mute : true;
}

void dbg(const char *str)  { if(!DBG_MUTE) { Serial.print(str);      Serial.flush(); } }
void dbg_c(char c)         { if(!DBG_MUTE) { Serial.print(c);        Serial.flush(); } }
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
    if      (val < 0x10)       { Serial.print("0x0000000"); }
    else if (val < 0x100)      { Serial.print("0x000000"); }
    else if (val < 0x1000)     { Serial.print("0x00000"); }
    else if (val < 0x10000)    { Serial.print("0x0000"); }
    else if (val < 0x100000)   { Serial.print("0x000"); }
    else if (val < 0x1000000)  { Serial.print("0x00"); }
    else if (val < 0x10000000) { Serial.print("0x0"); }
    else                       { Serial.print("0x"); }
    Serial.print(val, HEX);
    Serial.flush();
}

#endif  // NOT_ARDUINO

// Debug print into minimum width field
void dbg_w(const char *str, uint32_t width) {
    dbg(str);
    size_t len = strlen(str);
    for(int i = width-len; i>0; i--) {
        dbg(" ");
    }
}

// Decode address bits of SPI header
void dbg_decode_addr(uint16_t addr) {
    switch(addr) {
    case 0: dbg("Config_R"); break;
    case 1: dbg("ControlR"); break;
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

// Debug print decoded magic values into minimum width field (w)
void dbg_decode_u32(uint32_t val, uint32_t w) {
    switch(val) {
    case 0x09002000: dbg_w("SHARED_RAM_DEBUG_AREA", w); break;
    case 0x09004000: dbg_w("DOWNLOAD_FIFO_BASE", w);    break;
    case ADDR_DWL_CTRL_AREA_IMAGE_SIZE:  dbg_w("CTRL_AREA_IMAGE_SIZE", w);  break;
    case ADDR_DWL_CTRL_AREA_PUT:         dbg_w("CTRL_AREA_PUT", w);         break;
    case ADDR_DWL_CTRL_AREA_GET:         dbg_w("CTRL_AREA_GET", w);         break;
    case ADDR_DWL_CTRL_AREA_HOST_STATUS: dbg_w("CTRL_AREA_HOST_STATUS", w); break;
    case ADDR_DWL_CTRL_AREA_NCP_STATUS:  dbg_w("CTRL_AREA_NCP_STATUS", w);  break;
    case ADDR_DWL_CTRL_AREA_SIGNATURE:   dbg_w("CTRL_AREA_SIGNATURE", w);   break;
    case ADDR_DWL_CTRL_AREA_FW_HASH:     dbg_w("CTRL_AREA_FW_HASH", w);     break;
    case ADDR_DWL_CTRL_AREA_FW_VERSION:  dbg_w("CTRL_AREA_FW_VERSION", w);  break;
    case 0x12345678: dbg_w("*_STATE_NOT_READY", w);           break;
    case 0x7AB41D19: dbg_w("NCP_STATE_PUB_KEY_RDY", w);       break;
    case 0x87654321: dbg_w("*_STATE_READY", w);               break;
    case 0xA753BD99: dbg_w("HOST_STATE_HOST_INFO_READ", w);   break;
    case 0xABCDDCBA: dbg_w("*_STATE_*LOAD_PENDING", w);       break;
    case 0xBD53EF99: dbg_w("NCP_STATE_INFO_READY", w);        break;
    case 0xCAFEFECA: dbg_w("NCP_STATE_DOWNLOAD_COMPLETE", w); break;
    case 0xD4C64A99: dbg_w("*_STATE_(*_COMPLETE|*_OK)", w);   break;
    case 0xFFFFFFFF: dbg_w("*_STATE_UNDEF", w);               break;
    case HOST_STATE_OK_TO_JUMP:
        // Firmware load was successful, so unmute dbg trace
        dbg_set_mute(false);
        dbg_w("HOST_STATE_OK_TO_JUMP", w);
        break;
    default:
        dbg_hex32(val);
        dbg_w("", w-10);
    }
}

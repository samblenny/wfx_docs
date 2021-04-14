/**
 * Debug helper functions for POSIX/Arduino portability
 */
#include <stdio.h>
#include "dbg.h"

#ifdef NOT_ARDUINO
/**
 * For building with make, send debug to stdout with printf.
 */
void dbg(const char *str)    { printf("%s", str); }
void dbg_c(char c)           { printf("%c", c); }
void dbg_u16(uint16_t val)   { printf("%u", val); }
void dbg_u32(uint32_t val)   { printf("%u", val); }
void dbg_hex8(uint8_t val)   { printf("%02X", val); }
void dbg_hex16(uint16_t val) { printf("%04X", val); }
void dbg_hex32(uint32_t val) { printf("%08X", val); }

#else
/**
 * For building with the Arduino IDE, send debug to serial port.
 */
#include "Arduino.h"

void dbg(const char *str)  { Serial.print(str); }
void dbg_c(char c)         { Serial.print(c); }
void dbg_u16(uint16_t val) { Serial.print(val, DEC); }
void dbg_u32(uint32_t val) { Serial.print(val, DEC); }

void dbg_hex8(uint8_t val) {
    if (val < 0x10) {
        Serial.print("0");
    }
    Serial.print(val, HEX);
}

void dbg_hex16(uint16_t val) {
    if (val < 0x10) {
        Serial.print("000");
    } else if (val < 0x10) {
        Serial.print("00");
    } else if (val < 0x100) {
        Serial.print("0");
    }
    Serial.print(val, HEX);
}

void dbg_hex32(uint32_t val) {
    if (val < 0x10) {
        Serial.print("0000000");
    } else if (val < 0x100) {
        Serial.print("000000");
    } else if (val < 0x1000) {
        Serial.print("00000");
    } else if (val < 0x10000) {
        Serial.print("0000");
    } else if (val < 0x100000) {
        Serial.print("000");
    } else if (val < 0x1000000) {
        Serial.print("00");
    } else if (val < 0x10000000) {
        Serial.print("0");
    }
    Serial.print(val, HEX);
}

#endif

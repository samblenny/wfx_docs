#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * Debug helper functions for POSIX/Arduino portability
 */

void dbg(const char *str);
void dbg_w(const char *str, uint32_t width);
void dbg_c(char c);
void dbg_u8(uint8_t val);
void dbg_u16(uint16_t val);
void dbg_u32(uint32_t val);
void dbg_hex8(uint8_t val);
void dbg_hex16(uint16_t val);
void dbg_hex32(uint32_t val);
void dbg_decode_addr(uint16_t addr);
void dbg_decode_u32(uint32_t val, uint32_t width);

#ifdef __cplusplus
}
#endif

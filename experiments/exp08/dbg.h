#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * Debug helper functions for POSIX/Arduino portability
 */

void dbg(const char *str);
void dbg_c(char c);
void dbg_u16(uint16_t val);
void dbg_u32(uint32_t val);
void dbg_hex8(uint8_t val);
void dbg_hex16(uint16_t val);
void dbg_hex32(uint32_t val);

#ifdef __cplusplus
}
#endif

#include "firmware/sl_wfx_wf200_C0.h"
#include "PDS/brd8022a_pds.h"

// Firmware blob size
extern const uint32_t sl_wfx_firmware_size;

// Firmware blob
extern const uint8_t sl_wfx_firmware[];

// PDS string table (the SL header file uses static, so declare an exportable alias)
const char* const *pds_table = pds_table_brd8022a;

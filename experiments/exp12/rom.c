#include "firmware/sl_wfx_wf200_C0.h"
#include "PDS/brd8022a_pds.h"

// Firmware blob size
extern const uint32_t sl_wfx_firmware_size;

// Firmware blob
extern const uint8_t sl_wfx_firmware[];

// PDS table.
// This declaration is a bit odd because it implements a work-around for an
// inconvenient `static` declaration in the BRD8022a PDS table header file from
// Silicon Laboratories. The problem is that `static const` cannot be exported
// to the symbol table. Since I would prefer not to modify the header file, my
// work-around is to export an alias pointer for the PDS table.
#define _PDS_LINES (sizeof(pds_table_brd8022a) / sizeof(pds_table_brd8022a[0]))
const uint16_t pds_table_lines = _PDS_LINES;
const char* const *pds_table = pds_table_brd8022a;

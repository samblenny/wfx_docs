WFX_DRV=../wfx_fmac_driver
WFX_BUS=$(WFX_DRV)/bus
WFX_TOOLS=../wfx-fullMAC-tools

# These should be compiled together as the main object file (goal: run from RAM)
WFX_BUS_C=$(WFX_BUS)/sl_wfx_bus.c $(WFX_BUS)/sl_wfx_bus_spi.c
WFX_DRV_C=$(WFX_BUS_C) $(WFX_DRV)/sl_wfx.c
WFX_DRV_H=$(WFX_DRV)/sl_wfx.h

# These should be compiled together (goal: link as flash address, no RAM copy)
WFX_FW_H=$(WFX_DRV)/firmware/sl_wfx_wf200_C0.h
WFX_PDS_H=$(WFX_TOOLS)/PDS/brd8022a_pds.h

# There are a lot of flexible-array-extensions and newline-eof warnings in the wfx driver,
# and host_api.c has lots of unused-parameter warnings
CLANG_WARN=-Wall -Wextra -Wpedantic
CLANG_IGNORE=-Wno-flexible-array-extensions -Wno-newline-eof -Wno-unused-parameter

# This controls ifdef blocks in various sl_wfx* files, main.c, and dbg.cpp
# debug mask: 0x000f is init checkpoints + errors
DEBUG=-DNOT_ARDUINO -DSL_WFX_DEBUG_MASK=0x000f

NO_LINK=-c
CC_OPT=-Os
CC_STD=-std=c99

OUT_ALL=a.out
OUT_MAIN=main.o
OUT_ROM=rom.o

# With llvm v9+, this seems to work for testing riscv32 sizes:
# $ clang-9 -march=riscv32i --target=riscv32-unknown-unknown -c main.c
# $ objdump -s main.o

CC=clang $(CLANG_WARN) $(CLANG_IGNORE) $(CC_OPT) $(DEBUG)
OBJDUMP_HEADERS=objdump -h
OBJDUMP_SYM_TBL=objdump -t

all: main.c rom
	$(CC) -I $(WFX_DRV) $(WFX_DRV_C) $(OUT_ROM) -o $(OUT_ALL) host_api.c dbg.cpp main.c
	$(OBJDUMP_HEADERS) $(OUT_ALL)

rom: rom.c
	$(CC) $(NO_LINK) -I $(WFX_DRV) -I $(WFX_TOOLS) -o $(OUT_ROM) rom.c
	$(OBJDUMP_SYM_TBL) $(OUT_ROM)

.PHONY: clean
clean:
	rm -f $(OUT_ALL) $(OUT_MAIN) $(OUT_ROM)

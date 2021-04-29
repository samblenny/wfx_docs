WFX_DRV=../wfx_fmac_driver
WFX_BUS=$(WFX_DRV)/bus
WFX_TOOLS=../wfx-fullMAC-tools

# WFX fullMAC driver library code
WFX_BUS_C=$(WFX_BUS)/sl_wfx_bus.c $(WFX_BUS)/sl_wfx_bus_spi.c
WFX_DRV_C=$(WFX_BUS_C) $(WFX_DRV)/sl_wfx.c
WFX_DRV_H=$(WFX_DRV)/sl_wfx.h
# WFX firmware and PDS blobs
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

CC=clang $(CLANG_WARN) $(CLANG_IGNORE) $(CC_OPT) $(DEBUG)
MAIN_C=host_api.c dbg.cpp m4_spi.cpp main.c

all: main.c rom.o
	$(CC) -I $(WFX_DRV) $(WFX_DRV_C) -I $(WFX_TOOLS) rom.o $(MAIN_C)

rom.o: rom.c
	$(CC) $(NO_LINK) -I $(WFX_DRV) -I $(WFX_TOOLS) -o rom.o rom.c

run: all a.out
	./a.out

clean:
	rm -f a.out rom.o

.PHONY: clean

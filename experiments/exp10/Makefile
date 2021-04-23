CC=clang
LIB=../wfx_fmac_driver
BUS=$(LIB)/bus
BUS_C_FILES=$(BUS)/sl_wfx_bus.c $(BUS)/sl_wfx_bus_spi.c
INCLUDE_C=$(BUS_C_FILES) $(LIB)/sl_wfx.c
WARN=-Wall -Wextra -Wpedantic
# There are a lot of flexible-array-extensions and newline-eof warnings in the wfx driver,
# and host_api.c has lots of unused-parameter warnings
IGNORE=-Wno-flexible-array-extensions -Wno-newline-eof -Wno-unused-parameter

# This controls ifdef blocks in various sl_wfx* files, main.c, and dbg.cpp
# debug mask: 0x000f is init checkpoints + errors
# SL_WFX_DEBUG_MASK bits:
#   ERROR:   0x0001
#   INIT:    0x0002
#   SLEEP:   0x0004
#   SLK:     0x0008
#   RX:      0x0010
#   RX_RAW:  0x0020
#   RX_REG:  0x0040
#   TX:      0x0080
#   TX_RAW:  0x0100
#   TX_REG:  0x0200
#   FW_LOAD: 0x0400
DEBUG=-DNOT_ARDUINO -DSL_WFX_DEBUG_MASK=0x000f


all: main.c
	$(CC) -I $(LIB) $(WARN) $(IGNORE) $(DEBUG) $(INCLUDE_C) host_api.c dbg.cpp main.c

.PHONY: clean
clean:
	rm a.out

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
# debug mask: 0x07ff turns on everything, 0x040f is firmware + init checkpoints + errors
#DEBUG=-DNOT_ARDUINO -DSL_WFX_DEBUG_MASK=0x07ff
DEBUG=-DNOT_ARDUINO -DSL_WFX_DEBUG_MASK=0x040f


all: main.c
	$(CC) -I $(LIB) $(WARN) $(IGNORE) $(DEBUG) $(INCLUDE_C) host_api.c dbg.cpp main.c

.PHONY: clean
clean:
	rm a.out

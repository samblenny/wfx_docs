CC=clang
LIB=../wfx_fmac_driver
BUS=$(LIB)/bus
BUS_C_FILES=$(BUS)/sl_wfx_bus.c $(BUS)/sl_wfx_bus_spi.c
WARN=-Wall -Wextra -Wpedantic
# There are a lot of flexible-array-extensions and newline-eof warnings in the wfx driver,
# and host_api.c has lots of unused-parameter warnings
IGNORE=-Wno-flexible-array-extensions -Wno-newline-eof -Wno-unused-parameter

# This controls ifdef blocks in various sl_wfx*.h and sl_wfx*.c files
DEBUG=-DSL_WFX_DEBUG_MASK=0x007fff


all: main.c
	$(CC) -I $(LIB) $(WARN) $(IGNORE) $(DEBUG) main.c host_api.c $(BUS_C_FILES) $(LIB)/sl_wfx.c

.PHONY: clean
clean:
	rm a.out

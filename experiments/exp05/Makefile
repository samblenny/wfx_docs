LIB=../wfx_fmac_driver
BUS=$(LIB)/bus
BUS_C_FILES=$(BUS)/sl_wfx_bus.c $(BUS)/sl_wfx_bus_spi.c

# This controls ifdef blocks in various sl_wfx*.h and sl_wfx*.c files
DEBUG=-DSL_WFX_DEBUG_MASK=0x007fff

all: main.c
	gcc -I $(LIB) $(DEBUG) main.c host_api.c $(BUS_C_FILES) $(LIB)/sl_wfx.c

.PHONY: clean
clean:
	rm a.out

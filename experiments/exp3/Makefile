LIB=../wfx_fmac_driver
BUS=$(LIB)/bus
BUS_C_FILES=$(BUS)/sl_wfx_bus.c $(BUS)/sl_wfx_bus_spi.c

all: main.c
	gcc -I $(LIB) main.c $(BUS_C_FILES) $(LIB)/sl_wfx.c

.PHONY: clean
clean:
	rm a.out

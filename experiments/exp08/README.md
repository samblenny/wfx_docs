# exp08

Objectives:

1. Progress further through `sl_wfx_init()`

2. Reduce debug trace verbosity for easier console log comparison


## Console Log 1 (clang stdout)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[261478e](https://github.com/samblenny/wfx-fullMAC-driver/commit/261478edcfc367612d9cac2157ba2724cb818bd9)
(after fixing `bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`)

```
$ make
clang -I ../wfx_fmac_driver -Wall -Wextra -Wpedantic -Wno-flexible-array-extensions -Wno-newline-eof -Wno-unused-parameter -DNOT_ARDUINO -DSL_WFX_DEBUG_MASK=0x040f ../wfx_fmac_driver/bus/sl_wfx_bus.c ../wfx_fmac_driver/bus/sl_wfx_bus_spi.c ../wfx_fmac_driver/sl_wfx.c host_api.c dbg.cpp main.c
$ ./a.out
init_bus
reset_chip
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x56000404 -> W(addr:0,len:2,oBuf:0x04045600) iBuf:0x04000000; CS1
--Bus initialized--
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x6002 0x75872007 -> W(addr:6,len:2,oBuf:0x07208775) iBuf:0x75872007; CS1
CS0; spi_xfer 0x6002 0x20C02E08 -> W(addr:6,len:2,oBuf:0x082EC020) iBuf:0x20C02E08; CS1
CS0; spi_xfer 0x6002 0x3C3C3C09 -> W(addr:6,len:2,oBuf:0x093C3C3C) iBuf:0x3C3C3C09; CS1
CS0; spi_xfer 0x6002 0x442C320B -> W(addr:6,len:2,oBuf:0x0B322C44) iBuf:0x442C320B; CS1
CS0; spi_xfer 0x6002 0x9764A00C -> W(addr:6,len:2,oBuf:0x0CA06497) iBuf:0x9764A00C; CS1
CS0; spi_xfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; CS1
CS0; spi_xfer 0x1002 0x00300000 -> W(addr:1,len:2,oBuf:0x00003000) iBuf:0x00300000; CS1
CS0; spi_xfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
--Chip initialized--
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x8EC8AB23; CS1
CS0; spi_xfer 0x8002 0x8EC8AB23 -> R(addr:0,len:2,oBuf:0xAB238EC8) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0x8EC8AB23 -> W(addr:5,len:2,oBuf:0x23ABC88E) iBuf:0x8EC8AB23; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00040000 -> R(addr:5,len:2,oBuf:0x00000400) iBuf:0x8EC8AB23; CS1
--Bootloader running--
setup_waited_event
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
irq=off
deinit_bus
deinit
$
```


## Console Log 2 (Arduino IDE serial monitor)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[261478e](https://github.com/samblenny/wfx-fullMAC-driver/commit/261478edcfc367612d9cac2157ba2724cb818bd9)
(after fixing `bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`)

```
init_bus
reset_chip
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x56000404 -> W(addr:0,len:2,oBuf:0x04045600) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x6002 0x75872007 -> W(addr:6,len:2,oBuf:0x07208775) iBuf:0x75872007; CS1
CS0; spi_xfer 0x6002 0x20C02E08 -> W(addr:6,len:2,oBuf:0x082EC020) iBuf:0x20C02E08; CS1
CS0; spi_xfer 0x6002 0x3C3C3C09 -> W(addr:6,len:2,oBuf:0x093C3C3C) iBuf:0x3C3C3C09; CS1
CS0; spi_xfer 0x6002 0x442C320B -> W(addr:6,len:2,oBuf:0x0B322C44) iBuf:0x442C320B; CS1
CS0; spi_xfer 0x6002 0x9764A00C -> W(addr:6,len:2,oBuf:0x0CA06497) iBuf:0x9764A00C; CS1
CS0; spi_xfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; CS1
CS0; spi_xfer 0x1002 0x00300000 -> W(addr:1,len:2,oBuf:0x00003000) iBuf:0x00300000; CS1
CS0; spi_xfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x8EC8AB23; CS1
CS0; spi_xfer 0x8002 0x8EC8AB23 -> R(addr:0,len:2,oBuf:0xAB238EC8) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0x8EC8AB23 -> W(addr:5,len:2,oBuf:0x23ABC88E) iBuf:0x8EC8AB23; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00040000 -> R(addr:5,len:2,oBuf:0x00000400) iBuf:0x8EC8AB23; CS1
setup_waited_event
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
irq=off
deinit_bus
deinit
```

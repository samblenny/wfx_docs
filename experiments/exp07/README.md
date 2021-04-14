# exp07

Objectives:

1. Modify `sl_wfx_host_spi_transfer_no_cs_assert()` so read of config register
   returns non-zero, allowing `sl_wfx_init_bus()` to return `SL_STATUS_OK`,
   allowing `sl_wfx_init()` to continue beyond initializing the bus

2. Modify debug printing in `host_api.c` to work with both Darwin stdout and
   Arduino `Serial.print()`, which does not take printf style format strings


## Weirdness

Comparing console logs 1 and 2 below, the Arduino debug trace has an unexpected
different value of `spi_transfer 0x9002 0xC0FF0220`, instead of
`spi_transfer 0x9002 0x01000000` for Darwin, following the line with
`spi_transfer 0x6002 0x9764A00C`.

The odd values appear to be stack garbage that comes from an uninitialized array
used in `bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`, as called from
`sl_wfx.c::sl_wfx_init_chip()`.

Code Links (zero-buf branch at commit 8901519):
- [`sl_wfx.c::sl_wfx_init_chip()`](https://github.com/samblenny/wfx-fullMAC-driver/blob/890151953fa56da7370bfdc117043ab6de8fe3b8/wfx_fmac_driver/sl_wfx.c#L1949-L1957)
- [`bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`](https://github.com/samblenny/wfx-fullMAC-driver/blob/890151953fa56da7370bfdc117043ab6de8fe3b8/wfx_fmac_driver/bus/sl_wfx_bus.c#L28-L29)

Initializing the array in `sl_wfx_reg_read_16()` fixed the problem (see console
logs 3 and 4 below).


## Console Log 1 (Darwin stdout)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver

```
$ make
clang -I ../wfx_fmac_driver -Wall -Wextra -Wpedantic -Wno-flexible-array-extensions -Wno-newline-eof -Wno-unused-parameter -DNOT_ARDUINO -DSL_WFX_DEBUG_MASK=0x007fff ../wfx_fmac_driver/bus/sl_wfx_bus.c ../wfx_fmac_driver/bus/sl_wfx_bus_spi.c ../wfx_fmac_driver/sl_wfx.c host_api.c dbg.cpp main.c
$ ./a.out
init_bus
reset_chip
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
TX_REG> addr:00, 04045601
cs=on; spi_transfer 0x0002 0x56010404 -> W(addr:0,len:2,oBuf:0x04045601) iBuf:0x00010000; cs=off
--Bus initialized--
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
TX_REG> addr:06, 07208775
cs=on; spi_transfer 0x6002 0x75872007 -> W(addr:6,len:2,oBuf:0x07208775) iBuf:0x75872007; cs=off
TX_REG> addr:06, 082EC020
cs=on; spi_transfer 0x6002 0x20C02E08 -> W(addr:6,len:2,oBuf:0x082EC020) iBuf:0x20C02E08; cs=off
TX_REG> addr:06, 093C3C3C
cs=on; spi_transfer 0x6002 0x3C3C3C09 -> W(addr:6,len:2,oBuf:0x093C3C3C) iBuf:0x3C3C3C09; cs=off
TX_REG> addr:06, 0B322C44
cs=on; spi_transfer 0x6002 0x442C320B -> W(addr:6,len:2,oBuf:0x0B322C44) iBuf:0x442C320B; cs=off
TX_REG> addr:06, 0CA06497
cs=on; spi_transfer 0x6002 0x9764A00C -> W(addr:6,len:2,oBuf:0x0CA06497) iBuf:0x9764A00C; cs=off
cs=on; spi_transfer 0x9002 0x01000000 -> R(addr:1,len:2,oBuf:0x00000001) iBuf:0x01200000; cs=off
RX_REG> addr:01, 2001
TX_REG> addr:01, 3001
cs=on; spi_transfer 0x1002 0x01300000 -> W(addr:1,len:2,oBuf:0x00003001) iBuf:0x01300000; cs=off
cs=on; spi_transfer 0x9002 0x01000000 -> R(addr:1,len:2,oBuf:0x00000001) iBuf:0x01200000; cs=off
RX_REG> addr:01, 2001
cs=on; spi_transfer 0x8002 0x01000000 -> R(addr:0,len:2,oBuf:0x00000100) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
TX_REG> addr:00, 00000001
cs=on; spi_transfer 0x0002 0x00010000 -> W(addr:0,len:2,oBuf:0x00000001) iBuf:0x00010000; cs=off
irq=off
deinit_bus
deinit
$
```


## Console Log 2 (Arduino IDE serial monitor with uninitialized array bug)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit 8901519
(before the fix to `bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`)

```
init_bus
reset_chip
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x0002 0x56010404 -> W(addr:0,len:2,oBuf:0x04045601) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x6002 0x75872007 -> W(addr:6,len:2,oBuf:0x07208775) iBuf:0x75872007; cs=off
cs=on; spi_transfer 0x6002 0x20C02E08 -> W(addr:6,len:2,oBuf:0x082EC020) iBuf:0x20C02E08; cs=off
cs=on; spi_transfer 0x6002 0x3C3C3C09 -> W(addr:6,len:2,oBuf:0x093C3C3C) iBuf:0x3C3C3C09; cs=off
cs=on; spi_transfer 0x6002 0x442C320B -> W(addr:6,len:2,oBuf:0x0B322C44) iBuf:0x442C320B; cs=off
cs=on; spi_transfer 0x6002 0x9764A00C -> W(addr:6,len:2,oBuf:0x0CA06497) iBuf:0x9764A00C; cs=off
cs=on; spi_transfer 0x9002 0xC0FF0220 -> R(addr:1,len:2,oBuf:0x2002FFC0) iBuf:0xC0FF0220; cs=off
cs=on; spi_transfer 0x1002 0xC0FF0000 -> W(addr:1,len:2,oBuf:0x0000FFC0) iBuf:0xC0FF0000; cs=off
cs=on; spi_transfer 0x9002 0xC0FF0220 -> R(addr:1,len:2,oBuf:0x2002FFC0) iBuf:0xC0FF0220; cs=off
cs=on; spi_transfer 0x8002 0x01000000 -> R(addr:0,len:2,oBuf:0x00000100) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x0002 0x00010000 -> W(addr:0,len:2,oBuf:0x00000001) iBuf:0x00010000; cs=off
irq=off
deinit_bus
deinit
```


## Console Log 3 (Darwin stdout with zeroed array fix)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit 261478e,
after fixing uninitialized buffer in `bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`

```
$ make
clang -I ../wfx_fmac_driver -Wall -Wextra -Wpedantic -Wno-flexible-array-extensions -Wno-newline-eof -Wno-unused-parameter -DNOT_ARDUINO -DSL_WFX_DEBUG_MASK=0x007fff ../wfx_fmac_driver/bus/sl_wfx_bus.c ../wfx_fmac_driver/bus/sl_wfx_bus_spi.c ../wfx_fmac_driver/sl_wfx.c host_api.c dbg.cpp main.c
$ ./a.out
init_bus
reset_chip
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
TX_REG> addr:00, 04045601
cs=on; spi_transfer 0x0002 0x56010404 -> W(addr:0,len:2,oBuf:0x04045601) iBuf:0x00010000; cs=off
--Bus initialized--
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
TX_REG> addr:06, 07208775
cs=on; spi_transfer 0x6002 0x75872007 -> W(addr:6,len:2,oBuf:0x07208775) iBuf:0x75872007; cs=off
TX_REG> addr:06, 082EC020
cs=on; spi_transfer 0x6002 0x20C02E08 -> W(addr:6,len:2,oBuf:0x082EC020) iBuf:0x20C02E08; cs=off
TX_REG> addr:06, 093C3C3C
cs=on; spi_transfer 0x6002 0x3C3C3C09 -> W(addr:6,len:2,oBuf:0x093C3C3C) iBuf:0x3C3C3C09; cs=off
TX_REG> addr:06, 0B322C44
cs=on; spi_transfer 0x6002 0x442C320B -> W(addr:6,len:2,oBuf:0x0B322C44) iBuf:0x442C320B; cs=off
TX_REG> addr:06, 0CA06497
cs=on; spi_transfer 0x6002 0x9764A00C -> W(addr:6,len:2,oBuf:0x0CA06497) iBuf:0x9764A00C; cs=off
cs=on; spi_transfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; cs=off
RX_REG> addr:01, 2000
TX_REG> addr:01, 3000
cs=on; spi_transfer 0x1002 0x00300000 -> W(addr:1,len:2,oBuf:0x00003000) iBuf:0x00300000; cs=off
cs=on; spi_transfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; cs=off
RX_REG> addr:01, 2000
cs=on; spi_transfer 0x8002 0x01000000 -> R(addr:0,len:2,oBuf:0x00000100) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
RX_REG> addr:00, 00000001
TX_REG> addr:00, 00000001
cs=on; spi_transfer 0x0002 0x00010000 -> W(addr:0,len:2,oBuf:0x00000001) iBuf:0x00010000; cs=off
irq=off
deinit_bus
deinit
```


## Console Log 4 (Arduino IDE serial monitor with zeroed array fix)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit 261478e,
after fixing uninitialized buffer in `bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`

```
init_bus
reset_chip
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x0002 0x56010404 -> W(addr:0,len:2,oBuf:0x04045601) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x6002 0x75872007 -> W(addr:6,len:2,oBuf:0x07208775) iBuf:0x75872007; cs=off
cs=on; spi_transfer 0x6002 0x20C02E08 -> W(addr:6,len:2,oBuf:0x082EC020) iBuf:0x20C02E08; cs=off
cs=on; spi_transfer 0x6002 0x3C3C3C09 -> W(addr:6,len:2,oBuf:0x093C3C3C) iBuf:0x3C3C3C09; cs=off
cs=on; spi_transfer 0x6002 0x442C320B -> W(addr:6,len:2,oBuf:0x0B322C44) iBuf:0x442C320B; cs=off
cs=on; spi_transfer 0x6002 0x9764A00C -> W(addr:6,len:2,oBuf:0x0CA06497) iBuf:0x9764A00C; cs=off
cs=on; spi_transfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; cs=off
cs=on; spi_transfer 0x1002 0x00300000 -> W(addr:1,len:2,oBuf:0x00003000) iBuf:0x00300000; cs=off
cs=on; spi_transfer 0x9002 0x00000000 -> R(addr:1,len:2,oBuf:0x00000000) iBuf:0x00200000; cs=off
cs=on; spi_transfer 0x8002 0x01000000 -> R(addr:0,len:2,oBuf:0x00000100) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x00010000; cs=off
cs=on; spi_transfer 0x0002 0x00010000 -> W(addr:0,len:2,oBuf:0x00000001) iBuf:0x00010000; cs=off
irq=off
deinit_bus
deinit
```

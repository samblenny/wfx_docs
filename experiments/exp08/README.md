# exp08

Objectives:

1. Progress further through `sl_wfx_init()`

2. Reduce debug trace verbosity for easier console log comparison


## Weirdness

This is the `diff` between console logs 1 and 2:

```
5d4
< --Bus initialized--
16d14
< --Chip initialized--
31d28
< --Bootloader running--
46c43
< CS0; spi_xfer 0xD002 0x8024BCD1 -> R(addr:5,len:2,oBuf:0xD1BC2480) iBuf:0x99EF53BD; CS1
---
> CS0; spi_xfer 0xD002 0x80000000 -> R(addr:5,len:2,oBuf:0x00000080) iBuf:0x99EF53BD; CS1
63c60
< CS0; spi_xfer 0xD002 0xC424BCD1 -> R(addr:5,len:2,oBuf:0xD1BC24C4) iBuf:0x21436587; CS1
---
> CS0; spi_xfer 0xD002 0x80000000 -> R(addr:5,len:2,oBuf:0x00000080) iBuf:0x21436587; CS1
```

The problem was that `sl_wfx.c::sl_wfx_poll_for_value()` did `uint32_t value;` without
a `= 0` to initialize `value` before it was passed by reference to `sl_wfx_apb_read_32()`.
See [commit 261478e: sl_wfx.c#L2197-L2203](https://github.com/samblenny/wfx-fullMAC-driver/blob/261478edcfc367612d9cac2157ba2724cb818bd9/wfx_fmac_driver/sl_wfx.c#L2197-L2203)

Console logs 3 and 4 match because they use a fixed version of `sl_wfx_poll_for_value()`.
See [commit 536ccf0](https://github.com/samblenny/wfx-fullMAC-driver/commit/536ccf0d85fadcba5e7455af06875a9de7e91dfe)

This is the `diff` between console logs 3 and 4:

```
5d4
< --Bus initialized--
16d14
< --Chip initialized--
31d28
< --Bootloader running--
```

## Console Log 1 (clang stdout)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[261478e](https://github.com/samblenny/wfx-fullMAC-driver/commit/261478edcfc367612d9cac2157ba2724cb818bd9)
(after fixing `bus/sl_wfx_bus.c::sl_wfx_reg_read_16()`)

```
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
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
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
init
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x78563412 -> W(addr:5,len:2,oBuf:0x12345678) iBuf:0x78563412; CS1
CS0; spi_xfer 0x4002 0x04C00009 -> W(addr:4,len:2,oBuf:0x0900C004) iBuf:0x04C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x08C00009 -> W(addr:4,len:2,oBuf:0x0900C008) iBuf:0x08C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x21436587 -> W(addr:5,len:2,oBuf:0x87654321) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x8024BCD1 -> R(addr:5,len:2,oBuf:0xD1BC2480) iBuf:0x99EF53BD; CS1
CS0; spi_xfer 0x4002 0x80C00009 -> W(addr:4,len:2,oBuf:0x0900C080) iBuf:0x80C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0xCCC00009 -> W(addr:4,len:2,oBuf:0x0900C0CC) iBuf:0xCCC00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x99BD53A7 -> W(addr:5,len:2,oBuf:0xA753BD99) iBuf:0x99BD53A7; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0xC424BCD1 -> R(addr:5,len:2,oBuf:0xD1BC24C4) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0xFFFFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFFF) iBuf:0x8EC8AB23; CS1
get_firmware_size
CS0; spi_xfer 0x4002 0x00C00009 -> W(addr:4,len:2,oBuf:0x0900C000) iBuf:0x00C00009; CS1
CS0; spi_xfer 0x5002 0xB0FFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFB0) iBuf:0xB0FFFFFF; CS1
get_firmware
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
irq=off
deinit_bus
deinit
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
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0x8EC8AB23 -> W(addr:5,len:2,oBuf:0x23ABC88E) iBuf:0x8EC8AB23; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00040000 -> R(addr:5,len:2,oBuf:0x00000400) iBuf:0x8EC8AB23; CS1
setup_waited_event
init
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x78563412 -> W(addr:5,len:2,oBuf:0x12345678) iBuf:0x78563412; CS1
CS0; spi_xfer 0x4002 0x04C00009 -> W(addr:4,len:2,oBuf:0x0900C004) iBuf:0x04C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x08C00009 -> W(addr:4,len:2,oBuf:0x0900C008) iBuf:0x08C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x21436587 -> W(addr:5,len:2,oBuf:0x87654321) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x80000000 -> R(addr:5,len:2,oBuf:0x00000080) iBuf:0x99EF53BD; CS1
CS0; spi_xfer 0x4002 0x80C00009 -> W(addr:4,len:2,oBuf:0x0900C080) iBuf:0x80C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0xCCC00009 -> W(addr:4,len:2,oBuf:0x0900C0CC) iBuf:0xCCC00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x99BD53A7 -> W(addr:5,len:2,oBuf:0xA753BD99) iBuf:0x99BD53A7; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x80000000 -> R(addr:5,len:2,oBuf:0x00000080) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0xFFFFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFFF) iBuf:0x8EC8AB23; CS1
get_firmware_size
CS0; spi_xfer 0x4002 0x00C00009 -> W(addr:4,len:2,oBuf:0x0900C000) iBuf:0x00C00009; CS1
CS0; spi_xfer 0x5002 0xB0FFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFB0) iBuf:0xB0FFFFFF; CS1
get_firmware
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
irq=off
deinit_bus
deinit
```


## Console Log 3 (clang stdout with fix)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[536ccf0](https://github.com/samblenny/wfx-fullMAC-driver/commit/536ccf0d85fadcba5e7455af06875a9de7e91dfe)
(after fixing `sl_wfx_poll_for_value()`)

```
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
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
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
init
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x78563412 -> W(addr:5,len:2,oBuf:0x12345678) iBuf:0x78563412; CS1
CS0; spi_xfer 0x4002 0x04C00009 -> W(addr:4,len:2,oBuf:0x0900C004) iBuf:0x04C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x08C00009 -> W(addr:4,len:2,oBuf:0x0900C008) iBuf:0x08C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x21436587 -> W(addr:5,len:2,oBuf:0x87654321) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x99EF53BD; CS1
CS0; spi_xfer 0x4002 0x80C00009 -> W(addr:4,len:2,oBuf:0x0900C080) iBuf:0x80C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0xCCC00009 -> W(addr:4,len:2,oBuf:0x0900C0CC) iBuf:0xCCC00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x99BD53A7 -> W(addr:5,len:2,oBuf:0xA753BD99) iBuf:0x99BD53A7; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0xFFFFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFFF) iBuf:0x8EC8AB23; CS1
get_firmware_size
CS0; spi_xfer 0x4002 0x00C00009 -> W(addr:4,len:2,oBuf:0x0900C000) iBuf:0x00C00009; CS1
CS0; spi_xfer 0x5002 0xB0FFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFB0) iBuf:0xB0FFFFFF; CS1
get_firmware
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
irq=off
deinit_bus
deinit
```


## Console Log 4 (Arduino serial monitor with fix)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[536ccf0](https://github.com/samblenny/wfx-fullMAC-driver/commit/536ccf0d85fadcba5e7455af06875a9de7e91dfe)
(after fixing `sl_wfx_poll_for_value()`)

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
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0x8EC8AB23 -> W(addr:5,len:2,oBuf:0x23ABC88E) iBuf:0x8EC8AB23; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00040000 -> R(addr:5,len:2,oBuf:0x00000400) iBuf:0x8EC8AB23; CS1
setup_waited_event
init
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x78563412 -> W(addr:5,len:2,oBuf:0x12345678) iBuf:0x78563412; CS1
CS0; spi_xfer 0x4002 0x04C00009 -> W(addr:4,len:2,oBuf:0x0900C004) iBuf:0x04C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x08C00009 -> W(addr:4,len:2,oBuf:0x0900C008) iBuf:0x08C00009; CS1
CS0; spi_xfer 0x5002 0x00000000 -> W(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x21436587 -> W(addr:5,len:2,oBuf:0x87654321) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x99EF53BD; CS1
CS0; spi_xfer 0x4002 0x80C00009 -> W(addr:4,len:2,oBuf:0x0900C080) iBuf:0x80C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0xCCC00009 -> W(addr:4,len:2,oBuf:0x0900C0CC) iBuf:0xCCC00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x00000000; CS1
CS0; spi_xfer 0x4002 0x0CC00009 -> W(addr:4,len:2,oBuf:0x0900C00C) iBuf:0x0CC00009; CS1
CS0; spi_xfer 0x5002 0x99BD53A7 -> W(addr:5,len:2,oBuf:0xA753BD99) iBuf:0x99BD53A7; CS1
CS0; spi_xfer 0x4002 0x10C00009 -> W(addr:4,len:2,oBuf:0x0900C010) iBuf:0x10C00009; CS1
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x24000000 -> W(addr:0,len:2,oBuf:0x00002400) iBuf:0x04000000; CS1
CS0; spi_xfer 0x8002 0x00040000 -> R(addr:0,len:2,oBuf:0x00000004) iBuf:0x04000000; CS1
CS0; spi_xfer 0xD002 0x00000000 -> R(addr:5,len:2,oBuf:0x00000000) iBuf:0x21436587; CS1
CS0; spi_xfer 0x4002 0x00400009 -> W(addr:4,len:2,oBuf:0x09004000) iBuf:0x00400009; CS1
CS0; spi_xfer 0x5002 0xFFFFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFFF) iBuf:0x8EC8AB23; CS1
get_firmware_size
CS0; spi_xfer 0x4002 0x00C00009 -> W(addr:4,len:2,oBuf:0x0900C000) iBuf:0x00C00009; CS1
CS0; spi_xfer 0x5002 0xB0FFFFFF -> W(addr:5,len:2,oBuf:0xFFFFFFB0) iBuf:0xB0FFFFFF; CS1
get_firmware
CS0; spi_xfer 0x8002 0x00000000 -> R(addr:0,len:2,oBuf:0x00000000) iBuf:0x04000000; CS1
CS0; spi_xfer 0x0002 0x04000000 -> W(addr:0,len:2,oBuf:0x00000400) iBuf:0x04000000; CS1
irq=off
deinit_bus
deinit
```

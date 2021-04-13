# exp05

The objectives of this experiment are to turn on `wfx_fmac_driver`'s builtin
debug logging by defining `SL_WFX_DEBUG_MASK` and to begin exploring what
happens inside of `sl_wfx_init()`.

I added a primitve protocol decoder to `sl_wfx_host_spi_transfer_no_cs_assert()`
so it can try to supply SPI responses expected by `sl_wfx_init_chip()`. It
works well enough to avoid the 200 attempt long retry loop that reads the
control register looking for the RDY bit to be set.


## Console Log (uninitialized destination buffers)

This uses the include-paths branch of samblenny/wfx-fullMAC-driver (commit
[f0c5504](https://github.com/samblenny/wfx-fullMAC-driver/commit/f0c55043943b68beba38f1743a06fbd835243ca0)).

```bash
$ ls
Makefile   README.md  host_api.c main.c
$ make
gcc -I ../wfx_fmac_driver -DSL_WFX_DEBUG_MASK=0x007fff main.c host_api.c ../wfx_fmac_driver/bus/sl_wfx_bus.c ../wfx_fmac_driver/bus/sl_wfx_bus_spi.c ../wfx_fmac_driver/sl_wfx.c
$ ./a.out
init_bus
reset_chip
cs=on; spi_transfer 0x8002 0x48D05B02 -> R(addr:0,len:2,buf:5B0248D0); cs=off
RX_REG> addr:00, 5B0248D0
TX_REG> addr:00, 5F065ED0
cs=on; spi_transfer 0x0002 0x5ED05F06 -> W(addr:0,len:2,buf:5F065ED0); cs=off
--Bus initialized--
cs=on; spi_transfer 0x8002 0x8500B252 -> R(addr:0,len:2,buf:B2528500); cs=off
RX_REG> addr:00, B2528500
TX_REG> addr:06, 07208775
cs=on; spi_transfer 0x6002 0x75872007 -> W(addr:6,len:2,buf:07208775); cs=off
TX_REG> addr:06, 082EC020
cs=on; spi_transfer 0x6002 0x20C02E08 -> W(addr:6,len:2,buf:082EC020); cs=off
TX_REG> addr:06, 093C3C3C
cs=on; spi_transfer 0x6002 0x3C3C3C09 -> W(addr:6,len:2,buf:093C3C3C); cs=off
TX_REG> addr:06, 0B322C44
cs=on; spi_transfer 0x6002 0x442C320B -> W(addr:6,len:2,buf:0B322C44); cs=off
TX_REG> addr:06, 0CA06497
cs=on; spi_transfer 0x6002 0x9764A00C -> W(addr:6,len:2,buf:0CA06497); cs=off
cs=on; spi_transfer 0x9002 0x01000000 -> R(addr:1,len:2,buf:00000001) 0x01200000; cs=off
RX_REG> addr:01, 2001
TX_REG> addr:01, 3001
cs=on; spi_transfer 0x1002 0x01300000 -> W(addr:1,len:2,buf:00003001) 0x01300000; cs=off
cs=on; spi_transfer 0x9002 0x01000000 -> R(addr:1,len:2,buf:00000001) 0x01200000; cs=off
RX_REG> addr:01, 2001
cs=on; spi_transfer 0x8002 0x008552B2 -> R(addr:0,len:2,buf:52B20085); cs=off
RX_REG> addr:00, 52B20085
cs=on; spi_transfer 0x8002 0x01000000 -> R(addr:0,len:2,buf:00000100); cs=off
RX_REG> addr:00, 00000100
TX_REG> addr:00, 00000100
cs=on; spi_transfer 0x0002 0x01000000 -> W(addr:0,len:2,buf:00000100); cs=off
irq=off
deinit_bus
deinit
$
```


## Console Log (zeroed destination buffers)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver (commit
[8901519](https://github.com/samblenny/wfx-fullMAC-driver/commit/890151953fa56da7370bfdc117043ab6de8fe3b8)).

The control flow is different here because `bus/sl_wfx_bus_spi.c::sl_wfx_init_bus()`
considers a return value of `0` from reading the config register to be an error:
https://github.com/samblenny/wfx-fullMAC-driver/blob/zero-buf/wfx_fmac_driver/bus/sl_wfx_bus_spi.c#L96-L99

Running exp05 with the include-paths branch left the destination buffer for
reading the config register initialized with random garbage off the stack. In
the console log above, the value happened to be `0x48D05B02`, which is non-zero,
so the error check in `sl_wfx_init_bus()` did not get triggered.

With the zero-buf branch, the control flow looks like this:
```
$ make
gcc -I ../wfx_fmac_driver -Wall -Wextra -Wpedantic -Wno-flexible-array-extensions -Wno-newline-eof -Wno-unused-parameter -DSL_WFX_DEBUG_MASK=0x007fff main.c host_api.c ../wfx_fmac_driver/bus/sl_wfx_bus.c ../wfx_fmac_driver/bus/sl_wfx_bus_spi.c ../wfx_fmac_driver/sl_wfx.c
$ ./a.out
init_bus
reset_chip
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,buf:00000000); cs=off
RX_REG> addr:00, 00000000
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,buf:00000000); cs=off
RX_REG> addr:00, 00000000
TX_REG> addr:00, 00000000
cs=on; spi_transfer 0x0002 0x00000000 -> W(addr:0,len:2,buf:00000000); cs=off
irq=off
deinit_bus
deinit
$
```

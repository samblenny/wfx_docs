# exp05

The objectives of this experiment are to turn on `wfx_fmac_driver`'s builtin
debug logging by defining `SL_WFX_DEBUG_MASK` and to begin exploring what
happens inside of `sl_wfx_init()`.

I added a primitve protocol decoder to `sl_wfx_host_spi_transfer_no_cs_assert()`
so it can try to supply SPI responses expected by `sl_wfx_init_chip()`. It
works well enough to avoid the 200 attempt long retry loop that reads the
control register looking for the RDY bit to be set.


## Console Log

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

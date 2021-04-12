# Exp06

The objective of this experiment is to port exp05 to an Adafruit Feather M4
using the Arduino IDE and compare its output to the debug log of exp05.

Arduino's `Serial.print()` doesn't easily support `vprintf()` for implementing
`sl_wfx_host_log()`, so I did not attempt to compile with `wfx_fmac_driver`'s
built in debug logging enabled.


## Results

1. Non-deterministic SPI Reads: The buffer contents for register reads seem to
   be non-deterministic due to the use of uninitialized stack variables.

2. Early Termination: The debug log of the Feather M4 port ends much sooner
   than exp05. So far, I haven't figured out why.


## Arduino IDE Serial Monitor Log
```
init_bus
reset_chip
cs=on; spi_transfer 0x8002 0x00000000 -> R(addr:0,len:2,buf:00000000); cs=off
cs=on; spi_transfer 0x8002 0xC40B0020 -> R(addr:0,len:2,buf:0020C40B); cs=off
cs=on; spi_transfer 0x0002 0xC40B0020 -> W(addr:0,len:2,buf:0020C40B); cs=off
irq=off
deinit_bus
deinit
```

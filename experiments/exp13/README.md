# exp13

Objectives:

1. Get SPI working between Feather M4 and WF200 eval board

2. Power WF200 eval board through INA190EVM current sense amplifier

3. Trace WF200 eval board current with Saleae (just reset pulse, no SPI yet)


## Reset Pulse Current Sense Captures

[**UPDATE AFTER exp14:** *These captures are messed up because the WF200 eval
board was not getting power to `3V3` on the `EXP` header, so only part of the
reset circuit was receiving power*]

Saleae Logic 8 captures of 200V/V gain INA190 high-side current sense amplifier
inline with WF200 eval board VMCU.

WF200 Eval board has five 78kΩ pull-ups on the data lines and 9.9kΩ on RESETn,
but only RESETn was connected to the Feather M4 for this experiment. Saleae
Logic 8 calibrated voltage quantization step size appears to about 1.35mV,
corresponding to a current step size of 52µA (200 V/V gain across 0.130Ω).

| INA190 Vout | gain    | R       | I (Vout/gain/R) | Comment                            |
|-------------|---------|---------|-----------------|------------------------------------|
| 0.031 V     | 200 V/V | 0.130 Ω | 1.19 mA         | SiLabs AN1219: 75 µA for reset low |
| 0.143 V     | 200 V/V | 0.130 Ω | 5.50 mA         | SiLabs AN1219: 1.5 mA for snooze   |

![reset low 31mV](reset_pulse_capture_31mV.png)

![reset high 143mV](reset_pulse_capture_143mV.png)

<!--
## Console Log 1 (clang stdout)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[54842b4](https://github.com/samblenny/wfx-fullMAC-driver/commit/54842b4931d10a7119db337ba30ed2ff5dd6d959)

See [console_log_1.txt](console_log_1.txt)


## Console Log 2 (Arduino serial monitor)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[54842b4](https://github.com/samblenny/wfx-fullMAC-driver/commit/54842b4931d10a7119db337ba30ed2ff5dd6d959)

See [console_log_2_arduino.txt](console_log_2.txt)


## Console Log Diff

See [console_log_diff.txt](console_log_diff.txt)
-->

## PDS File Setup

The purpose of a PDS file is to configure the WF200 radio and GPIO. For the
BRD8022A "WF200 Wi-Fi EXP Board" evaluation board from Silicon Laboratories,
there is a header file for the pre-defined PDS at
https://github.com/SiliconLabs/wfx-fullMAC-tools/blob/master/PDS/brd8022a_pds.h

To get the PDS header file, do this, starting from `exp13/`:
```
cd ../
mkdir -p wfx-fullMAC-tools/PDS
cd wfx-fullMAC-tools/PDS
curl -O https://raw.githubusercontent.com/SiliconLabs/wfx-fullMAC-tools/master/PDS/brd8022a_pds.h
# and a workaround for the Arduino IDE library include path...
cd ../../exp13
mkdir PDS
ln -s ../../wfx-fullMAC-tools/PDS/brd8022a_pds.h PDS/brd8022a_pds.h
```

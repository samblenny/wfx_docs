# exp14

Objectives:

1. Get SPI working between Feather M4 and WF200 eval board

2. Trace WF200 eval board current with Saleae during firmware & PDS upload


## Current Sense Captures

Saleae Logic 8 captures of 200V/V gain INA190 high-side current sense amplifier
inline with WF200 eval board VMCU.

WF200 Eval board has five 78kΩ pull-ups on the data lines and 9.9kΩ on RESETn,
but only RESETn was connected to the Feather M4 for this experiment. Saleae
Logic 8 calibrated voltage quantization step size appears to about 1.35mV,
corresponding to a current step size of 5.2µA (200 V/V gain across 0.130Ω).

| INA190 Vout | gain    | R       | I (Vout/gain/R) | Comment                            |
|-------------|---------|---------|-----------------|------------------------------------|
| -- V        | 200 V/V | 0.130 Ω | -- mA           |                                    |
| -- V        | 200 V/V | 0.130 Ω | -- mA           |                                    |

<!--
![current sense capture](current_sense_capture.png)


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

To get the PDS header file, do this, starting from `exp14/`:
```
cd ../
mkdir -p wfx-fullMAC-tools/PDS
cd wfx-fullMAC-tools/PDS
curl -O https://raw.githubusercontent.com/SiliconLabs/wfx-fullMAC-tools/master/PDS/brd8022a_pds.h
# and a workaround for the Arduino IDE library include path...
cd ../../exp14
mkdir PDS
ln -s ../../wfx-fullMAC-tools/PDS/brd8022a_pds.h PDS/brd8022a_pds.h
```

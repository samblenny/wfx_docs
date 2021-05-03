# exp13

Objectives:

1. Get SPI working between Feather M4 and WF200 eval board

2. Power WF200 eval board through INA190EVM current sense amplifier

3. Trace WF200 eval board current with Saleae (just reset pulse, no SPI yet)

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

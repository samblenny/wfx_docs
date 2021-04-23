# exp09

Objectives:

1. Get pds loader working

2. Mute debug tracing before firmware okay-to-jump


## PDS File Setup

The purpose of a PDS file is to configure the WF200 radio and GPIO. For the
BRD8022A "WF200 Wi-Fi EXP Board" evaluation board from Silicon Laboratories,
there is a header file for the pre-defined PDS at
https://github.com/SiliconLabs/wfx-fullMAC-tools/blob/master/PDS/brd8022a_pds.h

To get the PDS header file, do this, starting from `exp10/`:
```
cd ../
mkdir -p wfx-fullMAC-tools/PDS
cd wfx-fullMAC-tools/PDS
curl -O https://raw.githubusercontent.com/SiliconLabs/wfx-fullMAC-tools/master/PDS/brd8022a_pds.h
cd ../../exp10
ln -s ../wfx-fullMAC-tools/PDS/brd8022a_pds.h brd8022a_pds.h
```


## Console Log 1 (clang stdout)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[54842b4](https://github.com/samblenny/wfx-fullMAC-driver/commit/54842b4931d10a7119db337ba30ed2ff5dd6d959)

See [console_log_1_clang.txt](console_log_1_clang.txt)


## Console Log 2 (Arduino serial monitor)

This uses the zero-buf branch of samblenny/wfx-fullMAC-driver at commit
[54842b4](https://github.com/samblenny/wfx-fullMAC-driver/commit/54842b4931d10a7119db337ba30ed2ff5dd6d959)

See [console_log_2_arduino.txt](console_log_2_arduino.txt)


## Console Log Diff

See [console_log_diff.txt](console_log_diff.txt)

# exp09

Objectives:

1. Convert Arduino build/flash/run from IDE button to Makefile

2. Split compilation so firmware image and PDS go in their own object files

3. Add Makefile target to calculate sizes of object files


## clang-9 rv32i* test

This works on Raspberry Pi OS Buster to test compile sizes of the various
rv32 instruction sets.

```
$ sudo apt install clang-9
$ clang-9 -march=rv32i --target=riscv32-unknown-unknown -fno-ident -O0 -c main.c
$ objdump -s main.o

main.o:     file format elf32-little

Contents of section .text:
 0000 130101fe 232e1100 232c8100 13040102  ....#...#,......
 0010 13050000 232aa4fe 13052000 2328a4fe  ....#*.... .#(..
 0020 13055000 2326a4fe 032504ff 8325c4fe  ..P.#&...%...%..
 0030 97000000 e7800000 1305c5ff 03248101  .............$..
 0040 8320c101 13010102 67800000           . ......g...
$ clang-9 -march=rv32im --target=riscv32-unknown-unknown -fno-ident -O0 -c main.c
$ objdump -s main.o

main.o:     file format elf32-little

Contents of section .text:
 0000 130101fe 232e1100 232c8100 13040102  ....#...#,......
 0010 13050000 232aa4fe 13052000 2328a4fe  ....#*.... .#(..
 0020 13055000 2326a4fe 032504ff 8325c4fe  ..P.#&...%...%..
 0030 3305b502 1305c5ff 03248101 8320c101  3........$... ..
 0040 13010102 67800000                    ....g...
$ clang-9 -march=rv32ima --target=riscv32-unknown-unknown -fno-ident -O0 -c main.c
$ objdump -s main.o

main.o:     file format elf32-little

Contents of section .text:
 0000 130101fe 232e1100 232c8100 13040102  ....#...#,......
 0010 13050000 232aa4fe 13052000 2328a4fe  ....#*.... .#(..
 0020 13055000 2326a4fe 032504ff 8325c4fe  ..P.#&...%...%..
 0030 3305b502 1305c5ff 03248101 8320c101  3........$... ..
 0040 13010102 67800000                    ....g...
$ clang-9 -march=rv32imac --target=riscv32-unknown-unknown -fno-ident -O0 -c main.c
$ objdump -s main.o

main.o:     file format elf32-little

Contents of section .text:
 0000 01000111 06ce22cc 00100145 232aa4fe  ......"....E#*..
 0010 09452328 a4fe1545 2326a4fe 032504ff  .E#(...E#&...%..
 0020 8325c4fe 3305b502 71156244 f2400561  .%..3...q.bD.@.a
 0030 8280                                 ..
$
```


## PDS File Setup

The purpose of a PDS file is to configure the WF200 radio and GPIO. For the
BRD8022A "WF200 Wi-Fi EXP Board" evaluation board from Silicon Laboratories,
there is a header file for the pre-defined PDS at
https://github.com/SiliconLabs/wfx-fullMAC-tools/blob/master/PDS/brd8022a_pds.h

To get the PDS header file, do this, starting from `exp11/`:
```
cd ../
mkdir -p wfx-fullMAC-tools/PDS
cd wfx-fullMAC-tools/PDS
curl -O https://raw.githubusercontent.com/SiliconLabs/wfx-fullMAC-tools/master/PDS/brd8022a_pds.h
```

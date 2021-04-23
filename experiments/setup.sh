#!/bin/sh
# Prepare the wfx_fmac_driver library directory
git clone https://github.com/samblenny/wfx-fullMAC-driver.git -b zero-buf
ln -s wfx-fullMAC-driver/wfx_fmac_driver wfx_fmac_driver
mkdir -p wfx-fullMAC-tools/PDS
cd wfx-fullMAC-tools/PDS
curl -O https://raw.githubusercontent.com/SiliconLabs/wfx-fullMAC-tools/master/PDS/brd8022a_pds.h
cd ../..

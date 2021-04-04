#!/bin/sh
# Prepare the wfx_fmac_driver library directory
git clone --depth 2 https://github.com/samblenny/wfx-fullMAC-driver.git -b include-paths
cd wfx-fullMAC-driver
ln -s wfx-fullMAC-driver/wfx_fmac_driver wfx_fmac_driver

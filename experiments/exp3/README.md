# exp3

The objective of this experiment is to call a function from sl_wfx.h,
compile main.c and the wfx_fmac_driver library, and get linker errors
about missing symbols for unimplemented `sl_wfx_host_*` API functions.


## Console Log

```bash
$ ls
Makefile  README.md main.c
$ make
gcc -I ../wfx_fmac_driver main.c ../wfx_fmac_driver/bus/sl_wfx_bus.c ../wfx_fmac_driver/bus/sl_wfx_bus_spi.c ../wfx_fmac_driver/sl_wfx.c
Undefined symbols for architecture x86_64:
  "_sl_wfx_host_allocate_buffer", referenced from:
      _sl_wfx_allocate_command_buffer in sl_wfx-b340ba.o
      _sl_wfx_receive_frame in sl_wfx-b340ba.o
  "_sl_wfx_host_deinit", referenced from:
      _sl_wfx_init in sl_wfx-b340ba.o
  "_sl_wfx_host_deinit_bus", referenced from:
      _sl_wfx_deinit_bus in sl_wfx_bus_spi-4bcf88.o
  "_sl_wfx_host_disable_platform_interrupt", referenced from:
      _sl_wfx_disable_irq in sl_wfx-b340ba.o
  "_sl_wfx_host_enable_platform_interrupt", referenced from:
      _sl_wfx_enable_irq in sl_wfx-b340ba.o
  "_sl_wfx_host_free_buffer", referenced from:
      _sl_wfx_free_command_buffer in sl_wfx-b340ba.o
  "_sl_wfx_host_get_firmware_data", referenced from:
      _sl_wfx_download_run_firmware in sl_wfx-b340ba.o
  "_sl_wfx_host_get_firmware_size", referenced from:
      _sl_wfx_download_run_firmware in sl_wfx-b340ba.o
  "_sl_wfx_host_get_pds_data", referenced from:
      _sl_wfx_init in sl_wfx-b340ba.o
  "_sl_wfx_host_get_pds_size", referenced from:
      _sl_wfx_init in sl_wfx-b340ba.o
  "_sl_wfx_host_init", referenced from:
      _sl_wfx_download_run_firmware in sl_wfx-b340ba.o
  "_sl_wfx_host_init_bus", referenced from:
      _sl_wfx_init_bus in sl_wfx_bus_spi-4bcf88.o
  "_sl_wfx_host_lock", referenced from:
      _sl_wfx_send_request in sl_wfx-b340ba.o
      _sl_wfx_receive_frame in sl_wfx-b340ba.o
  "_sl_wfx_host_post_event", referenced from:
      _sl_wfx_receive_frame in sl_wfx-b340ba.o
  "_sl_wfx_host_reset_chip", referenced from:
      _sl_wfx_init_bus in sl_wfx_bus_spi-4bcf88.o
  "_sl_wfx_host_set_wake_up_pin", referenced from:
      _sl_wfx_bus_access in sl_wfx_bus-ead98c.o
      _sl_wfx_init in sl_wfx-b340ba.o
      _sl_wfx_shutdown in sl_wfx-b340ba.o
      _sl_wfx_enable_device_power_save in sl_wfx-b340ba.o
      _sl_wfx_disable_device_power_save in sl_wfx-b340ba.o
  "_sl_wfx_host_setup_waited_event", referenced from:
      _sl_wfx_init in sl_wfx-b340ba.o
      _sl_wfx_send_request in sl_wfx-b340ba.o
  "_sl_wfx_host_sleep_grant", referenced from:
      _sl_wfx_bus_access in sl_wfx_bus-ead98c.o
  "_sl_wfx_host_spi_cs_assert", referenced from:
      _sl_wfx_reg_read in sl_wfx_bus_spi-4bcf88.o
      _sl_wfx_reg_write in sl_wfx_bus_spi-4bcf88.o
  "_sl_wfx_host_spi_cs_deassert", referenced from:
      _sl_wfx_reg_read in sl_wfx_bus_spi-4bcf88.o
      _sl_wfx_reg_write in sl_wfx_bus_spi-4bcf88.o
  "_sl_wfx_host_spi_transfer_no_cs_assert", referenced from:
      _sl_wfx_reg_read in sl_wfx_bus_spi-4bcf88.o
      _sl_wfx_reg_write in sl_wfx_bus_spi-4bcf88.o
  "_sl_wfx_host_transmit_frame", referenced from:
      _sl_wfx_send_request in sl_wfx-b340ba.o
  "_sl_wfx_host_unlock", referenced from:
      _sl_wfx_send_request in sl_wfx-b340ba.o
      _sl_wfx_receive_frame in sl_wfx-b340ba.o
  "_sl_wfx_host_wait", referenced from:
      _sl_wfx_apb_read_32 in sl_wfx_bus-ead98c.o
      _sl_wfx_init_chip in sl_wfx-b340ba.o
      _sl_wfx_poll_for_value in sl_wfx-b340ba.o
  "_sl_wfx_host_wait_for_confirmation", referenced from:
      _sl_wfx_init in sl_wfx-b340ba.o
      _sl_wfx_send_configuration in sl_wfx-b340ba.o
      _sl_wfx_send_command in sl_wfx-b340ba.o
      _sl_wfx_send_join_command in sl_wfx-b340ba.o
      _sl_wfx_start_ap_command in sl_wfx-b340ba.o
      _sl_wfx_update_ap_command in sl_wfx-b340ba.o
      _sl_wfx_send_scan_command in sl_wfx-b340ba.o
      ...
  "_sl_wfx_host_wait_for_wake_up", referenced from:
      _sl_wfx_bus_access in sl_wfx_bus-ead98c.o
      _sl_wfx_disable_device_power_save in sl_wfx-b340ba.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make: *** [all] Error 1
$
```

## List of Unimplemented Host API Functions

```
sl_wfx_host_allocate_buffer
sl_wfx_host_deinit
sl_wfx_host_deinit_bus
sl_wfx_host_disable_platform_interrupt
sl_wfx_host_enable_platform_interrupt
sl_wfx_host_free_buffer
sl_wfx_host_get_firmware_data
sl_wfx_host_get_firmware_size
sl_wfx_host_get_pds_data
sl_wfx_host_get_pds_size
sl_wfx_host_init
sl_wfx_host_init_bus
sl_wfx_host_lock
sl_wfx_host_post_event
sl_wfx_host_reset_chip
sl_wfx_host_set_wake_up_pin
sl_wfx_host_setup_waited_event
sl_wfx_host_sleep_grant
sl_wfx_host_spi_cs_assert
sl_wfx_host_spi_cs_deassert
sl_wfx_host_spi_transfer_no_cs_assert
sl_wfx_host_transmit_frame
sl_wfx_host_unlock
sl_wfx_host_wait
sl_wfx_host_wait_for_confirmation
sl_wfx_host_wait_for_wake_up
```

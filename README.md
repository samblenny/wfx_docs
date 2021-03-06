# wfx_docs
unofficial WF200 documentation

## Experiments

- [exp18: SSID Scan Results](experiments/exp18)
- [exp17: HAL and SSID Scan](experiments/exp17)
- [exp16](experiments/exp16)
- [exp15](experiments/exp15)
- [exp14](experiments/exp14)
- [exp13](experiments/exp13)
- ... (see [experiments/](experiments))


## Links to Official Docs

1. This one explains requirements for the `wfx_host_api_*` SPI bus function
   implementations: https://docs.silabs.com/wifi/wf200/rtos/latest/wf200-hif

2. This one explains about implementing a loop to receive frames along with
   how to implement `sl_wfx_host_post_event()` and
   `sl_wfx_host_wait_for_confirmation()` for the event notification queue:
   https://docs.silabs.com/wifi/wf200/rtos/latest/driver-host-requirements

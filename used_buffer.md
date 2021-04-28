I may be mis-reading the code, but it looks like `sl_wfx_send_configuration()`
can increment `sl_wfx_context->used_buffers` in a way that may not get reliably
decremented. There is an error handler check that will decrement `used_buffers`
in the case of `SL_STATUS_TIMEOUT`, but nothing appears to decrement
`used_buffers` if `sl_wfx_host_wait_for_confirmation()` returns `SL_STATUS_OK`.

In `sl_wfx_send_configuration()`, the value of `sl_wfx_context->used_buffers`
[gets incremented](https://github.com/SiliconLabs/wfx-fullMAC-driver/blob/v3.3.2/wfx_fmac_driver/sl_wfx.c#L1627)
inside of a
[call to `sl_wfx_send_request()`](https://github.com/SiliconLabs/wfx-fullMAC-driver/blob/v3.3.2/wfx_fmac_driver/sl_wfx.c#L1267).

If `sl_wfx_send_configuration()` does not decrement
`sl_wfx_context->used_buffers`, then the consequence comes with subsequent code
calling `sl_wfx_send_request()`, which uses `unused_buffers` to check for
`SL_STATUS_WOULD_OVERFLOW`
([sl_wfx.c#L1604-L1607](https://github.com/SiliconLabs/wfx-fullMAC-driver/blob/v3.3.2/wfx_fmac_driver/sl_wfx.c#L1604-L1607)).

In that comparison above, the value of `sl_wfx_input_buffer_number`
[is copied by `sl_wfx_init()`](https://github.com/SiliconLabs/wfx-fullMAC-driver/blob/v3.3.2/wfx_fmac_driver/sl_wfx.c#L154-L155)
from `startup_info->body.num_inp_ch_bufs`. It may be possible that
`sl_wfx_send_request()` can return `SL_STATUS_WOULD_OVERFLOW` when the WFX
firmware still has unused buffers.

The code at the end of sl_wfx_send_configuration(), where success does not
decrement `used_buffers`, looks like this:
([sl_wfx.c#L1270-L1280](https://github.com/SiliconLabs/wfx-fullMAC-driver/blob/v3.3.2/wfx_fmac_driver/sl_wfx.c#L1270-L1280))

For comparison, `sl_wfx_receive_frame()` decrements
`sl_wfx_context->used_buffers` like this:
([sl_wfx.c#L1704-L1712](https://github.com/SiliconLabs/wfx-fullMAC-driver/blob/v3.3.2/wfx_fmac_driver/sl_wfx.c#L1704-L1712))

Other functions that call `sl_wfx_send_request()` (incrementing `used_buffers`)
include:
- `sl_wfx_send_join_command()`
- `sl_wfx_start_ap_command()`
- `sl_wfx_update_ap_command()`
- `sl_wfx_send_ethernet_frame()`
- `sl_wfx_send_scan_command()`
- `sl_wfx_set_roam_parameters()`
- `sl_wfx_ext_auth()`
- `sl_wfx_shutdown()`
- `sl_wfx_send_command()`

As far as I can tell, the only things that can potentially decrement
`used_buffers` are the `SL_STATUS_TIMEOUT` error handler checks and
`sl_wfx_receive_frame()`

## Conclusion

Perhaps the host_api is meant to be implemented such that
`sl_wfx_host_wait_for_confirmation()` will always make a call to
`sl_wfx_receive_frame()`.

#include "sl_wfx.h"

#ifdef NOT_ARDUINO
int main() {
    sl_wfx_status_t status;
    sl_wfx_context_t context;
    status = sl_wfx_init(&context);
    status = sl_wfx_deinit();
    return 0;
}
#endif

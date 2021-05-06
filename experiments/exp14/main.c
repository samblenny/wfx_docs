#include "sl_wfx.h"


extern void remember_context_ptr(sl_wfx_context_t *);

#ifdef NOT_ARDUINO
int main() {
    sl_wfx_status_t status;
    sl_wfx_context_t context;
    remember_context_ptr(&context);
    status = sl_wfx_init(&context);
    status = sl_wfx_deinit();
    return 0;
}
#endif

#include "rp2040_perf.h"
#include "target_syssvc.h"

void rp2040_hist_init(intptr_t exinf)
{
    sil_wrw_mem(RP2040_PWM_CSR(RP2040_HIST_PWM_CH), 0); /* Stop and initialize settings */
    sil_wrw_mem(RP2040_PWM_DIV(RP2040_HIST_PWM_CH), 1 << 4); /* Division by 1 */
    sil_wrw_mem(RP2040_PWM_TOP(RP2040_HIST_PWM_CH), 0xFFFF); /* 16bit period */
    sil_wrw_mem(RP2040_PWM_CSR(RP2040_HIST_PWM_CH), 1); /* Enable and let the counter run */
}

/*
 *		システムサービスのターゲット依存部
 *
 *  システムサービスのターゲット依存部のヘッダファイル．システムサービ
 *  スのターゲット依存の設定は，できる限りコンポーネント記述ファイルで
 *  記述し，このファイルに記述するものは最小限とする．
 * 
 *  $Id: target_syssvc.h 1074 2018-11-25 01:32:04Z ertl-hiro $
 */

#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H

/*
 *  コアで共通な定義（チップ依存部は飛ばす）
 */
#include "core_syssvc.h"

#define PMCR_D 3
#define PMCR_C 2
#define PMCR_E 0
#define PMCNTENSET_C 31


Inline void
pmon_start_cycle_counter()
{
    unsigned long x;

    x = 1 << PMCNTENSET_C;
    asm volatile("mcr	p15, 0, %0, c9, c12, 1" :: "r" (x));

    asm volatile("mrc	p15, 0, %0, c9, c12, 0" : "=r" (x));
    x |= ((1 << PMCR_C) | (1 << PMCR_E));
    x &= ~(1 << PMCR_D);
    asm volatile("mcr	p15, 0, %0, c9, c12, 0" :: "r" (x));
}

Inline uint32_t
pmon_read_cycle_counter()
{
    unsigned long x;
    asm volatile ("mrc	p15, 0, %0, c9, c13, 0": "=r" (x));
    return x;
}

Inline uint32_t
pmon_cnv_nsec_cycle(uint32_t time)
{
    /* 667Mhz(1.49ns) */
    return ((time) * 149) / 100;
}

#undef HIST_BM_HOOK
#define HIST_BM_HOOK() (pmon_start_cycle_counter())

#undef HISTTIM
#define HISTTIM uint32_t

#undef HIST_GET_TIM
#define HIST_GET_TIM(p_time)  (*p_time = pmon_read_cycle_counter());

#undef HIST_CONV_TIM
#define HIST_CONV_TIM(time)   (pmon_cnv_nsec_cycle(time))

#endif /* TOPPERS_TARGET_SYSSVC_H */

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
#include "mpcore.h"

Inline uint64_t
mpcore_gtc_get_count1(void)
{
	uint32_t	count_l, count_u, prev_count_u;

	count_u = sil_rew_mem(MPCORE_GTC_COUNT_U);
	do {
		prev_count_u = count_u;
		count_l = sil_rew_mem(MPCORE_GTC_COUNT_L);
		count_u = sil_rew_mem(MPCORE_GTC_COUNT_U);
	} while (count_u != prev_count_u);
	return((((uint64_t) count_u) << 32) | ((uint64_t) count_l));
}

#undef HIST_BM_HOOK
#define HIST_BM_HOOK()

#undef HISTTIM
#define HISTTIM uint64_t

Inline void
x_get_gtm(uint64_t *time) {
    *time = mpcore_gtc_get_count1();
}

Inline  uint64_t
x_cnv_nsec_gtm(uint64_t count) {
    return (count * (1000U / MPCORE_GTC_FREQ));
}
    
#undef HIST_GET_TIM
#define HIST_GET_TIM(p_time)  ((void) x_get_gtm(p_time));

#undef HIST_CONV_TIM
#define HIST_CONV_TIM(time)   (x_cnv_nsec_gtm(time))


#endif /* TOPPERS_TARGET_SYSSVC_H */

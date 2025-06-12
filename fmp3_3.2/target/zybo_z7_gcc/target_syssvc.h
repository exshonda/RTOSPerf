/*
 *		システムサービスのターゲット依存部（ZYBO_Z7用）
 *
 *  システムサービスのターゲット依存部のヘッダファイル．システムサービ
 *  スのターゲット依存の設定は，できる限りコンポーネント記述ファイルで
 *  記述し，このファイルに記述するものは最小限とする．
 * 
 *  $Id: target_syssvc.h 166 2019-08-28 07:54:41Z ertl-honda $
 */

#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H

#ifdef TOPPERS_OMIT_TECS

#include "zybo_z7.h"
#include "zynq7000.h"

/*
 *  起動メッセージのターゲットシステム名
 */
#define TARGET_NAME    "ZYBO_Z7 <Zynq-7000, Cortex-A9>"

/*
 *  シリアルインタフェースドライバを実行するクラスの定義
 */
#define CLS_SERIAL		CLS_PRC1

/*
 *  システムログの低レベル出力のための文字出力
 *
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．
 */
extern void	target_fput_log(char c);

/*
 *  サポートするシリアルポートの数
 */
#define TNUM_PORT		1

/*
 *  SIOドライバで使用するXUartPsに関する設定
 */
#define SIO_XUARTPS_BASE	ZYNQ_UART1_BASE
#define SIO_XUARTPS_MODE	XUARTPS_MR_CHARLEN_8 \
								| XUARTPS_MR_PARITY_NONE | XUARTPS_MR_STOPBIT_1
#define SIO_XUARTPS_BAUDGEN	XUARTPS_BAUDGEN_115K
#define SIO_XUARTPS_BAUDDIV	XUARTPS_BAUDDIV_115K

/*
 *  SIO割込みを登録するための定義
 */
#define INTNO_SIO		ZYNQ_UART1_IRQ		/* SIO割込み番号 */
#define ISRPRI_SIO		1					/* SIO ISR優先度 */
#define INTPRI_SIO		(-4)				/* SIO割込み優先度 */
#define INTATR_SIO		TA_NULL				/* SIO割込み属性 */

/*
 *  低レベル出力で使用するSIOポート
 */
#define SIOPID_FPUT		1

#endif /* TOPPERS_OMIT_TECS */

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

#if 0
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
#endif

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

/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2013 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 */
#ifndef TOPPERS_CHIP_GTIMER_H
#define TOPPERS_CHIP_GTIMER_H

#include <sil.h>
#include "mpcore.h"

/*
 *  Global Timer 下位32bitのみを使用
 */

/*
 *  Global Timer interrupts number, priority and attributes
 */
#define INHNO_TIMER_PRC1     (0x00010000 | IRQNO_MPCORE_GTM)
#define INHNO_TIMER_PRC2     (0x00020000 | IRQNO_MPCORE_GTM)
#define INHNO_TIMER_PRC3     (0x00030000 | IRQNO_MPCORE_GTM)
#define INHNO_TIMER_PRC4     (0x00040000 | IRQNO_MPCORE_GTM)

#define INTNO_TIMER_PRC1     (0x00010000 | IRQNO_MPCORE_GTM)
#define INTNO_TIMER_PRC2     (0x00020000 | IRQNO_MPCORE_GTM)
#define INTNO_TIMER_PRC3     (0x00030000 | IRQNO_MPCORE_GTM)
#define INTNO_TIMER_PRC4     (0x00040000 | IRQNO_MPCORE_GTM)


#define INTATR_TIMER   0U

#ifndef TOPPERS_MACRO_ONLY

/*
 *  Type definition for the internal representation of the timer value
 */
typedef uint32_t CLOCK;

/*
 *  Convertion between the internal representation of the timer value
 *  and miliseconds/microseconds. One tick equals to 1ms.
 */
#define TIMER_CLOCK          MPCORE_GTM_LOAD_1MS // cycles
#define TO_CLOCK(nume, deno) (TIMER_CLOCK * (nume) / (deno))
#define TO_USEC(clock)       (((SYSUTM) clock) * 1000U / TIMER_CLOCK)

/*
 *  Maximum timer value (internal representation in cycles)
 */
#define MAX_CLOCK        ((CLOCK) 0xffffffffU)

/*
 *  Approximate amount of time since the timer interrupt occurred
 */
#define GET_TOLERANCE    100U    /* delay tolerance (internal represent.) */
#define BEFORE_IREQ(clock) \
((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  Timer initialization
 */
extern void target_timer_initialize(intptr_t exinf);

/*
 * Timer termination
 */
extern void target_timer_terminate(intptr_t exinf);

/*
 *  Get the current timer value
 */
Inline CLOCK
target_timer_get_current(void)
{
	uint32_t count_l, count_h_p, count_h;
	uint64_t count;
	uint32_t comp_l, comp_h;
	uint64_t comp;
	uint32_t cyc = TO_CLOCK(TIC_NUME, TIC_DENO);
    
	/*
	 * 32bitカウンタ2個で64bitカウンタを実現されているため，
	 * 桁上げを考慮した読み込みとする．
	 */
	count_h_p = sil_rew_mem((void *)MPCORE_GTM_COUNT_U);
	count_l = sil_rew_mem((void *)MPCORE_GTM_COUNT_L);
	count_h = sil_rew_mem((void *)MPCORE_GTM_COUNT_U);
	if (count_h_p != count_h) {
		count_l = sil_rew_mem((void *)MPCORE_GTM_COUNT_L);
	}

	count = count_h;
	count = count << 32;
	count += count_l;

	comp_l = sil_rew_mem((void *)MPCORE_GTM_COMPV_L);
	comp_h = sil_rew_mem((void *)MPCORE_GTM_COMPV_U);    
	comp = comp_h;
	comp = comp << 32;
	comp += comp_l;

	return((uint32_t)(cyc - (comp - count)));
}

/*
 *  Check for pending timer interrupts
 */
Inline bool_t
target_timer_probe_int(void)
{
	if((sil_rew_mem((void *)MPCORE_GTM_ISR) & MPCORE_GTM_ISR_EF) == MPCORE_GTM_ISR_EF) {
		return true;
	} else {
		return false;
	}
}

/*
 *  Timer interrupt handler
 */
extern void target_timer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CHIP_TIMER_H */

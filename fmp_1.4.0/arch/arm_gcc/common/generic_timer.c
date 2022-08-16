/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2006-2016 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: generic_timer.c 1319 2019-01-22 02:50:40Z ertl-honda $
 */

/*
 *  タイマドライバ（ARMv7 Generic Timer用）
 */

#include "kernel_impl.h"
#include "time_event.h"
#include "target_timer.h"

/*
 *  タイマの周波数を保持する変数
 *  単位はkHz
 *  target_initialize() で初期化される
 */
uint32_t timer_clock;

/*
 *  タイマの設定値
 */
uint64_t timer_cval[TNUM_PRCID];

/*
 *  タイマの起動処理
 */
void
target_timer_initialize(intptr_t exinf)
{
	CLOCK cyc;
	uint32_t cval_l, cval_h;

	/*
	 *  タイマの停止
	 */
	CNTP_CTL_WRITE((uint32_t)0);

	/*
	 *  1ティックのクロック数
	 */
	cyc = TO_CLOCK(TIC_NUME, TIC_DENO);

	/*
	 *  CNTP_CVAL設定(Counter値 + cyc)
	 */
	CNTP_TVAL_WRITE((uint32_t)cyc);

	/*
	 *  タイマの開始
	 */
	CNTP_CTL_WRITE((uint32_t)CNTP_CTL_ENABLE_BIT);

	/*  
	 *  次のタイムアウト時のカウンタ値の取得
	 */
	CNTP_CVAL_READ(cval_l, cval_h);

	timer_cval[x_prc_index()] = ((uint64_t)cval_h << 32) | cval_l;
}

/*
 *  タイマの停止処理
 */
void
target_timer_terminate(intptr_t exinf)
{
	/*
	 *  タイマの停止
	 */
#if defined(TOPPERS_SAFEG_SECURE) && (__TARGET_ARCH_ARM == 7)
	CNTPS_CTL_WRITE((uint32_t)0);
#else /* !(defined(TOPPERS_SAFEG_SECURE) && (__TARGET_ARCH_ARM == 7)) */
	CNTP_CTL_WRITE((uint32_t)0);
#endif /* defined(TOPPERS_SAFEG_SECURE) && (__TARGET_ARCH_ARM == 7) */
}

/*
 *  タイマ割込みハンドラ
 */
void
target_timer_handler(void)
{
	ID prcid;
	CLOCK cyc;

	iget_pid(&prcid);

	/*
	 *  1ティックのクロック数
	 */
	cyc = TO_CLOCK(TIC_NUME, TIC_DENO);

	/*
	 *  次のタイムアウトのカウンタ値の設定
	 */
	timer_cval[prcid-1] = timer_cval[prcid-1] + (uint64_t)cyc;

	/*
	 *  タイマの設定
	 */
#if defined(TOPPERS_SAFEG_SECURE) && (__TARGET_ARCH_ARM == 7)
	CNTPS_CVAL_WRITE((uint32_t)timer_cval[prcid-1], (uint32_t)(timer_cval[prcid-1]>>32));
#else /* !(defined(TOPPERS_SAFEG_SECURE) && (__TARGET_ARCH_ARM == 7)) */
	CNTP_CVAL_WRITE((uint32_t)timer_cval[prcid-1], (uint32_t)(timer_cval[prcid-1]>>32));
#endif /* defined(TOPPERS_SAFEG_SECURE) && (__TARGET_ARCH_ARM == 7) */
	i_begin_int((0x10000 * prcid)|IRQNO_GTIM);
	signal_time();				/* タイムティックの供給 */
	i_end_int((0x10000 * prcid)|IRQNO_GTIM);
}

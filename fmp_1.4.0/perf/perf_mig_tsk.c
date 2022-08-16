/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2008-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: perf_mig_tsk.c 1210 2017-04-25 05:39:11Z ertl-honda $
 */

/*
 *  mig_tsk 性能測定プログラム
 */

#include <kernel.h>
#include <t_syslog.h>
#include <test_lib.h>
#include <sil.h>
#include <histogram.h>
#include "kernel_cfg.h"
#include "perf_mig_tsk.h"
#include "target_test.h"

/*
 *  計測回数と実行時間分布を記録する最大時間
 */
#define NO_MEASURE	20000U			/* 計測回数 */
#define MAX_TIME	400000U			/* 実行時間分布を記録する最大時間 */

/*
 *  実行時間分布を記録するメモリ領域
 */
static uint_t	histarea1[MAX_TIME + 1];

/*
 *  計測の前後でのフックルーチン
 */
#ifndef CPU1_PERF_PRE_HOOK
#define CPU1_PERF_PRE_HOOK
#endif  /* CPU1_PERF_PRE_HOOK */
#ifndef CPU1_PERF_POST_HOOK
#define CPU1_PERF_POST_HOOK
#endif  /* CPU1_PERF_POST_HOOK */
#ifndef CPU2_PERF_PRE_HOOK
#define CPU2_PERF_PRE_HOOK
#endif  /* CPU2_PERF_PRE_HOOK */
#ifndef CPU2_PERF_POST_HOOK
#define CPU2_PERF_POST_HOOK
#endif  /* CPU2_PERF_POST_HOOK */

void task1_3(intptr_t exinf)
{
	T_RTSK	rtsk;

	ref_tsk(TASK1_3,&rtsk);
	assert(rtsk.prcid == 1);
	begin_measure(1);
	mig_tsk(TASK1_3,2);
	end_measure(1);
	ref_tsk(TASK1_3,&rtsk);
	assert(rtsk.prcid == 2);
	ext_tsk();
}

void task1_5(intptr_t exinf)
{
	;
}

void task1_7(intptr_t exinf)
{
	T_RTSK   rtsk;

	end_measure(1);
	ref_tsk(TASK1_7,&rtsk);
	assert(rtsk.prcid == 2);
	ext_tsk();
}

void task2_1(intptr_t exinf)
{
	T_RTSK   rtsk;
	uint_t   i;

	for(i=0;i<NO_MEASURE;i++){
		PRCID_WAIT(TASK1_5,2); //TASK1_5がプロセッサ2にくるまで待つ
		ref_tsk(TASK1_5,&rtsk);
		assert(rtsk.prcid == 2);
		mig_tsk(TASK1_5,1);
	}
}

/*
 * 計測ルーチン
 */
void perf_eval(uint_t n)
{
	uint_t	 i;
	T_RTSK   rtsk;

	init_hist(1, MAX_TIME, histarea1);
	syslog_flush();
	dly_tsk(1000);
	CPU1_PERF_PRE_HOOK;

	for ( i = 0; i < NO_MEASURE; i++ ) {
		switch (n) {
			//オーバーヘッドの測定
		case 0:
			begin_measure(1);
			end_measure(1);
			break;

			//【1】対象タスクが休止状態
		case 1:
			PRCID_WAIT(TASK1_5,1); //TASK1_5がプロセッサ１にくるまで待つ
			ref_tsk(TASK1_5,&rtsk);
			assert(rtsk.prcid == 1);
			begin_measure(1);
			mig_tsk(TASK1_5,2);
			end_measure(1);
			break;

			//【2】対象タスクが実行可能状態
		case 2:
			STAT_WAIT(TASK1_7,TTS_DMT);
			mact_tsk(TASK1_7,1);
			ref_tsk(TASK1_7,&rtsk);
			assert(rtsk.prcid == 1);
			begin_measure(1);
			mig_tsk(TASK1_7,2);
			break;

			//【3】対象タスクが実行状態
		case 3:
			mact_tsk(TASK1_3,1);
			STAT_WAIT(TASK1_3,TTS_DMT);
			break;
		}
	}

	CPU1_PERF_POST_HOOK;

	syslog(LOG_NOTICE, "==================================");
	syslog(LOG_NOTICE, "(%d)", n);
	syslog(LOG_NOTICE, "----------------------------------");
	print_hist(1);
	test_finish();
}

/*
 *  PE1 メインタスク：中優先度
 */
void main_task1(intptr_t exinf)
{
	slp_tsk();

	perf_eval(0);
	perf_eval(1);
	perf_eval(2);
	perf_eval(3);

	wup_tsk(MAIN_TASK2);
}

/*
 *  PE2 メインタスク：中優先度
 */
void main_task2(intptr_t exinf)
{
#ifndef G_SYSLOG
	sus_tsk(LOGTASK2);
#endif /* G_SYSLOG */
	CPU2_PERF_PRE_HOOK;
	wup_tsk(MAIN_TASK1);
	slp_tsk();
	CPU2_PERF_POST_HOOK;
#ifndef G_SYSLOG
	rsm_tsk(LOGTASK2);
#endif /* G_SYSLOG */
}

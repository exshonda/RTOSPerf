/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2015 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: safeg_load.c 1210 2017-04-25 05:39:11Z ertl-honda $
 */

#include <kernel.h>
#include <t_stdlib.h>

static bool_t run_trust[TNUM_PRCID];
static SYSUTM trust_start_time[TNUM_PRCID];
static SYSUTM trust_total_time[TNUM_PRCID];
static SYSUTM load_start_time[TNUM_PRCID];

/*
 *  初期化
 */
void
safeg_load_init(intptr_t exinf)
{
	ID prcid;
	uint_t index;

	sil_get_pid(&prcid);
	index = prcid - 1;

	run_trust[index] = true;

	if (sns_ker()) {
		/* カーネル起動前 */
		trust_start_time[index] = 0;
		load_start_time[index] = 0;
	}
	else {
		/* カーネル起動後 */
		get_utm(&(trust_start_time[index]));
		load_start_time[index] = trust_start_time[index];
	}

	trust_total_time[index] = 0;
}

/*
 *  CPU利用率の取得
 */
void
safeg_load_info(ID prcid, uint32_t *p_trust_load, uint32_t *p_nontrust_load)
{
	SYSUTM cur_time;
	SYSUTM trust_total;
	uint_t index = prcid - 1;

	get_utm(&cur_time);
	trust_total = trust_total_time[index] + (cur_time - trust_start_time[index]);

	*p_trust_load = (trust_total/10)/((cur_time - load_start_time[index])/1000);
	*p_nontrust_load = 100 - *p_trust_load;
}

/*
 *  割込みの入口で呼び出す関数
 */
void
safeg_load_int(void){
	ID prcid;
	uint_t index;

	sil_get_pid(&prcid);
	index = prcid - 1;

	if (!run_trust[index]) {
		run_trust[index] = true;
		get_utm(&(trust_start_time[index]));
	}
}

/*
 *  アイドル時に呼び出す関数
 */
void
safeg_load_idle(void){
	SYSUTM time;
	ID prcid;
	uint_t index;

	sil_get_pid(&prcid);
	index = prcid - 1;
	
	run_trust[index] = false;
	get_utm(&time);
	trust_total_time[index] += time - trust_start_time[index];
}

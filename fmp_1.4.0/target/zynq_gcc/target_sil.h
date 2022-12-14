/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2007-2013 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  @(#) $Id: target_sil.h 1067 2014-12-24 14:15:10Z ertl-honda $
 */

/*
 *  This file is only included by sil.h just after including
 *  the header file t_stddef.h. It provides constants for the
 *  system interface layer (SIL)
 */
#ifndef TOPPERS_TARGET_SIL_H
#define TOPPERS_TARGET_SIL_H

/*
 *  Endianess of the processor
 */
#define SIL_ENDIAN_LITTLE

/*
 *  MPCore Private Memory Region Base Address (Table 4.7 in ZYNQ manual)
 */
#define SIL_MPCORE_PMR_BASE  0xF8F00000

/*
 *  Customize the SIL_DLY_NSE function for QEMU
 */
#ifdef TOPPERS_ENABLE_QEMU

#define TOPPERS_CUSTOM_SILDLYNSE
#ifndef TOPPERS_MACRO_ONLY
Inline void
sil_dly_nse(ulong_t dlytim)
{
	volatile int i;
	for(i=0; i < 10*dlytim; i++);
}
#endif /* TOPPERS_MACRO_ONLY */

#else

/*
 *  Definitions for very small waiting times
 */
#define SIL_DLY_TIM1    1
#define SIL_DLY_TIM2    1

#endif /* TOPPERS_ENABLE_QEMU */

/*
 *  Include chip definitions
 */
#include "chip_sil.h"

#endif /* TOPPERS_TARGET_SIL_H */

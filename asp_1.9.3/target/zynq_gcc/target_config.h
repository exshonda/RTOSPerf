/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.h 2828 2017-07-06 13:28:56Z ertl-honda $
 */

/*
 *  チップ依存モジュール（Zynq用）
 *
 *  カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  ターゲット依存部のハードウェア資源の定義
 */
#include "zynq.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  ASPカーネル動作時のメモリマップと関連する定義
 *
 *  0x00000000 - 0x0FFFFFFF : DRAM 
 *  0x40000000 - 0x43FFFFFF : NOR flash
 *  0x44000000 - 0x47FFFFFF : DOC flash
 *  0x48000000 - 0x4BFFFFFF : SRAM
 *  0x70000000 - 0x7FFFFFFF : DRAM(mirror)
 */

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#define DEFAULT_ISTKSZ      0x2000U

#if    defined(CORE0)
#define DEFAULT_ISTK        (void *)(0x02000000U - DEFAULT_ISTKSZ)
#elif  defined(CORE1)
#define DEFAULT_ISTK        (void *)(0x03000000U - DEFAULT_ISTKSZ)
#elif  defined(CORE2)
#define DEFAULT_ISTK        (void *)(0x04000000U - DEFAULT_ISTKSZ)
#elif  defined(CORE3)
#define DEFAULT_ISTK        (void *)(0x05000000U - DEFAULT_ISTKSZ)
#endif

/*
 *  SYS_PUTCで使用するUARTの指定
 */
#if    defined(CORE0)
#define SYS_PUTC_USER_UART0
#elif  defined(CORE1)
#define SYS_PUTC_USER_UART1
#elif  defined(CORE2)
#define SYS_PUTC_USER_UART2
#elif  defined(CORE3)
#define SYS_PUTC_USER_UART3
#endif

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    20
#define SIL_DLY_TIM2    10

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */
extern void	target_initialize(void);

/*
 * メモリマップの初期化（chip依存部から呼び出される）
 * 同時にキャッシュも有効にする
 */
extern void target_mmu_init(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void	target_exit(void) NoReturn;

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  チップ依存モジュール（MPCORE用）
 */
#include "arm_gcc/mpcore/chip_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */

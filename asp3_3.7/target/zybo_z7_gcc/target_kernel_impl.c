/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2007-2023 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: target_kernel_impl.c 1816 2023-04-18 05:44:37Z ertl-honda $
 */

/*
 *		カーネルのターゲット依存部（ZYBO用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "arm.h"
#include "zybo_z7.h"
#include "pl310.h"

/*
 *  カーネル動作時のメモリマップと関連する定義
 *
 *  0x00000000 - 0x1fffffff：外付けDDR（512MB）
 *  0x40000000 - 0xbfffffff：プログラマブルロジック領域
 *  0xe0000000 - 0xfdffffff：周辺デバイス等
 *  0xfffc0000 - 0xffffffff：オンチップメモリ（OCM）領域
 *								（上位番地にマッピングした場合）
 */

/*
 *  MMUへの設定属性（第1レベルディスクリプタ）
 */
#define MMU_ATTR_RAM	(ARM_MMU_DSCR1_SHARED|ARM_MMU_DSCR1_TEX001 \
							|ARMV6_MMU_DSCR1_AP011|ARM_MMU_DSCR1_CB11)
#define MMU_ATTR_IODEV	(ARM_MMU_DSCR1_SHARED|ARM_MMU_DSCR1_TEX000 \
							|ARMV6_MMU_DSCR1_AP011|ARM_MMU_DSCR1_CB01 \
							|ARMV6_MMU_DSCR1_NOEXEC)

/*
 *  外付けDDR領域の先頭番地，サイズ，属性
 */
#define DDR_ADDR		0x00000000
#define DDR_SIZE		0x20000000		/* 512MB */
#define DDR_ATTR		MMU_ATTR_RAM

/*
 *  プログラマブルロジック領域の先頭番地，サイズ，属性
 */
#define PL_ADDR			0x40000000
#define PL_SIZE			0x80000000		/* 2GB */
#define PL_ATTR			MMU_ATTR_IODEV

/*
 *  周辺デバイス等領域の先頭番地，サイズ，属性
 */
#define PERI_ADDR		0xe0000000
#define PERI_SIZE		0x1e000000
#define PERI_ATTR		MMU_ATTR_IODEV

/*
 *  オンチップメモリ領域の先頭番地，サイズ，属性
 *
 *  オンチップメモリの実際のサイズは256KBであるが，セクションテーブル
 *  では1MB単位でしか設定できないため，1MB単位に丸めて登録する．
 */
#define OCM_ADDR		0xfff00000
#define OCM_SIZE		0x00100000		/* 1MB */
#define OCM_ATTR		MMU_ATTR_RAM

/*
 *  MMUの設定情報（メモリエリアの情報）
 */
__attribute__((weak))
const ARM_MMU_CONFIG arm_memory_area[] = {
	{ DDR_ADDR,  DDR_ADDR,  DDR_SIZE,  DDR_ATTR  },
	{ PL_ADDR,   PL_ADDR,   PL_SIZE,   PL_ATTR   },
	{ PERI_ADDR, PERI_ADDR, PERI_SIZE, PERI_ATTR },
	{ OCM_ADDR,  OCM_ADDR,  OCM_SIZE,  OCM_ATTR  }
};

/*
 *  MMUの設定情報の数（メモリエリアの数）
 */
__attribute__((weak))
const uint_t arm_tnum_memory_area
					= sizeof(arm_memory_area) / sizeof(ARM_MMU_CONFIG);

/*
 *  システムログの低レベル出力のための初期化
 *
 *  セルタイプtPutLogSIOPort内に実装されている関数を直接呼び出す．
 */
extern void	tPutLogSIOPort_initialize(void);

/*
 *  ハードウェアの初期化
 */
void
hardware_init_hook(void)
{
	uint32_t reg;

	/* 
	 * キャッシュの無効化処理 
	 */
	CP15_READ_SCTLR(reg);
	if (reg & CP15_SCTLR_DCACHE) {
		/* Dキャッシュが有効な場合 */
		arm_clean_dcache();
		arm_clean_outer_cache();
	arm_disable_dcache();
	}
	/* MMUの無効化 */
	arm_disable_mmu();
	/* L2キャッシュの無効化 */
	arm_disable_outer_cache();
	/* Iキャッシュの無効化 */
	arm_disable_icache();
}

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	extern void	*vector_table;		/* ベクタテーブル */

	/*
	 *  チップ依存の初期化
	 */
	chip_initialize();

	/*
	 *  ベクタテーブルの設定
	 */
	CP15_WRITE_VBAR((uint32_t) &vector_table);

	/*
	 *  SIOを初期化
	 */
#ifndef TOPPERS_OMIT_TECS
	tPutLogSIOPort_initialize();
#endif /* TOPPERS_OMIT_TECS */
}

/*
 *  デフォルトのsoftware_term_hook（weak定義）
 */
__attribute__((weak))
void software_term_hook(void)
{
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
	/*
	 *  software_term_hookの呼出し
	 */
	software_term_hook();

	/*
	 *  チップ依存の終了処理
	 */
	chip_terminate();

	/*
	 *  ターゲット依存の終了処理
	 */
#if defined(TOPPERS_USE_QEMU) && !defined(TOPPERS_OMIT_QEMU_SEMIHOSTING)
	/*
	 *  QEMUを終了させる．
	 */
	Asm("ldr r1, =#0x20026\n\t"		/* ADP_Stopped_ApplicationExit */ 
		"mov r0, #0x18\n\t"			/* angel_SWIreason_ReportException */
		"svc 0x00123456");
#endif
	while (true) ;
}

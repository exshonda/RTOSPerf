/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2022 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: start.S 1755 2022-12-23 05:52:20Z ertl-hiro $
 */

/*
 *		カーネル用のスタートアップモジュール（ARM用）
 */
	
#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#define UINT_C(val)			(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)		(val)		/* ulong_t型の定数を作るマクロ */
#define CAST(type, val)		(val)		/* 型キャストを行うマクロ */
#include "kernel_impl.h"
#include "core_asm.inc"

/*
 *  使用する命令セットの定義
 */
#if defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE)
	.fpu	ASM_ARM_FPU_TYPE
#endif /* defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE) */

	ATEXT
	AALIGN(2)
	AGLOBAL(start)
ALABEL(start)
	/*
	 *  プロセッサモードの初期化
	 *
	 *  スーパバイザモード，全割込みロック状態に初期化する．
	 */
	msr		cpsr_cxsf, #(CPSR_SVC_MODE AOR CPSR_INTLOCK)

	/*
	 *  スタックポインタとフレームポインタの初期化
	 */
	ldr		r0, =_kernel_istkpt
	ldr		sp, [r0]			/* スタックポインタ（sp）*/
	mov		fp, #0				/* ARMモード用フレームポインタ（r11）*/
	mov		r7, #0				/* Thumbモード用フレームポインタ（r7）*/

#if defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE)
	/*
	 *  FPU（CP10とCP11）をアクセス可能に設定する．
	 *
	 *	CP15_READ_CPACR(reg);
	 *	reg |= (CP15_CPACR_CP10_FULLACCESS | CP15_CPACR_CP11_FULLACCESS);
	 *	CP15_WRITE_CPACR(reg);
	 */
	mrc		p15, 0, r0, c1, c0, 2
	orr		r0, r0, #(CP15_CPACR_CP10_FULLACCESS | CP15_CPACR_CP11_FULLACCESS)
	mcr		p15, 0, r0, c1, c0, 2
	asm_inst_sync_barrier r0
#endif /* defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE) */

#ifdef USE_ARM_FPU_ALWAYS
	/*
	 *  FPUをイネーブルする．
	 *
	 *	set_fpexc(current_fpexc() | FPEXC_ENABLE);
	 */
	vmrs	r0, fpexc
	orr		r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPUをディスエーブルする．
	 *
	 *	set_fpexc(current_fpexc() & ~FPEXC_ENABLE);
	 */
	vmrs	r0, fpexc
	bic		r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0
#endif /* USE_ARM_FPU_SELECTIVE */

	/*
	 *  hardware_init_hookの呼出し
	 *
	 *  ターゲットハードウェアに依存して必要な初期化処理がある場合は，
	 *  hardware_init_hookという関数を用意する．
	 */
	bl		hardware_init_hook

#ifndef TOPPERS_OMIT_BSS_INIT
	/*
	 *  bssセクションのクリア
	 *
	 *  BSS_STARTからBSS_ENDまでをゼロクリアする．
	 */
	ldr		r0, =BSS_START
	ldr		r2, =BSS_END
	cmp		r0, r2
	bhs		start_3
	mov		r1, #0
#ifdef TOPPERS_USE_STDLIB
	sub		r2, r2, r0
	bl		memset
#else /* TOPPERS_USE_STDLIB */
ALABEL(start_2)
	str		r1, [r0], #4
	cmp		r0, r2
	blo		start_2
#endif /* TOPPERS_USE_STDLIB */
ALABEL(start_3)
#endif /* TOPPERS_OMIT_BSS_INIT */

#ifndef TOPPERS_OMIT_DATA_INIT
	/*
	 *  dataセクションの初期化（ROM化対応）
	 *
	 *  DATA_STARTからDATA_ENDまでに，IDATA_START以降のデータをコピーす
	 *  る．
	 */
	ldr		r0, =DATA_START
	ldr		r2, =DATA_END
	cmp		r0, r2
	bhs		start_5
	ldr		r1, =IDATA_START
#ifdef TOPPERS_USE_STDLIB
	sub		r2, r2, r0
	bl		memcpy
#else /* TOPPERS_USE_STDLIB */
ALABEL(start_4)
	ldr		r3, [r1], #4
	str		r3, [r0], #4
	cmp		r0, r2
	blo		start_4
#endif /* TOPPERS_USE_STDLIB */
ALABEL(start_5)
#endif /* TOPPERS_OMIT_DATA_INIT */

	/*
	 *  software_init_hookの呼出し
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処理が
	 *  ある場合は，software_init_hookという関数を用意する．
	 */
	bl		software_init_hook

	/*
	 *  カーネルの起動
	 */
	b		sta_ker

/*
 *  デフォルトのhardware_init_hook（weak定義）
 */
	ATEXT
	AALIGN(2)
	AWEAK(hardware_init_hook)
ALABEL(hardware_init_hook)
	bx		lr

/*
 *  デフォルトのsoftware_init_hook（weak定義）
 */
	ATEXT
	AALIGN(2)
	AWEAK(software_init_hook)
ALABEL(software_init_hook)
	bx		lr

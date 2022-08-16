/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2019 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: atf_support.S 1314 2019-01-20 15:22:49Z ertl-honda $
 */
#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#include "kernel_impl.h"
#include "target_asm.inc"

#define ATF_SAVE_REGS_SIZE  (11 * 4)

    .bss
    AALIGN(4)

/*
 * ATFのSMC実行時に使用されるレジスタの値を保存する領域
 *
 *  R0 :    Function ID
 *  R1-R3 : Parameter
 *  R4-14 : Scratch registers
 */
    AGLOBAL(atf_smc_uses_regs)
ALABEL(atf_smc_uses_regs)
    .skip   ATF_SAVE_REGS_SIZE * TNUM_PRCID


/*
 * [Macro] Save AFT SMC uses regiters
 *
 * Parameter (Registers) :
 *   None
 *
 * Return value (Registers) :
 *   None
 */
    .macro save_atf_regs
    /* Get core ID */
    mrc   p15, 0, r12, c0, c0, 5     /* Read MPIDR */
    and   r12, r12, #0xFF
    /* Set save address to r4 */
    mov   r11, #ATF_SAVE_REGS_SIZE
    mul   r12, r12, r11
    ldr   r11, =atf_smc_uses_regs
    add   r12, r12, r11
    /* Get value of CPSR */
    mrs   r11, cpsr
    /* Save registers */
    stmib r12!, {r1-r11}
    .endm

/*
 * [Macro] Restore AFT SMC uses regiters
 *
 * Parameter (Registers) :
 *   None
 *
 * Return value (Registers) :
 *   None
 */
    .macro restore_atf_regs
    /* Get core ID */
    mrc   p15, 0, r12, c0, c0, 5     /* Read MPIDR */
    and   r12, r12, #0xFF
    /* Set save address to r4 */
    mov   r11, #ATF_SAVE_REGS_SIZE
    mul   r12, r12, r11
    ldr   r11, =atf_smc_uses_regs
    add   r12, r12, r11
    /* Restore registers */
    ldmib r12!, {r1-r11}
    /* Restore CPSR, SPSR */
    msr   spsr_fsxc, r11
    msr   cpsr, r11
    .endm

    ATEXT

#ifdef TARGET_FIQ_HANDLER
/*
 *  FIQハンドラ
 */
    AALIGN(4)
    AGLOBAL(fiq_handler)
ALABEL(fiq_handler)
    b   irq_handler
#endif /* TARGET_FIQ_HANDLER */

/*
 * ATFのTrusted OSベクタテーブル
 */
    AALIGN(4)
    AGLOBAL(atf_vector_table)
ALABEL(atf_vector_table)
    b   start                 /* PSCI CPU_ONでCPUコアON直後に実行されるコールバック */
    b   atf_fiq_entry

/*
 * ATFのTrusted OSベクタエントリ
 */
    AGLOBAL(atf_fiq_entry)
ALABEL(atf_fiq_entry)
    /* SPD SMCサービルコール実行前のレジスタを復帰 */
    restore_atf_regs
    /* 復帰アドレスを設定
     *   ATF(TOPPERS SPD)はBL33でBL32で処理する割込が発生したときにBL32に
     *  制御を移す。 その時、戻りアドレスをR0で受け渡す。
     *   AArch32では、IRQ発生時にLRが4加算される。 ここはSMCコール後の処理
     *  なので、LRは加算されない。
     *   irq_handler()は、 LRが4加算されることを考慮して実装されているため、
     *  辻褄を合わせるため、LRに4加算する。 */
    add lr, r0, #4
    /* atf_smc_int(), atf_smc_switch()の戻り値を設定 */
    mov r0, #0
    /* IRQハンドラへジャンプ */
    b   irq_handler

/*
 * ATFのNon-secure OS ダミーエントリ
 */
    AGLOBAL(atf_ns_dummy_entry)
ALABEL(atf_ns_dummy_entry)
    b   atf_ns_dummy_entry

/*
 *  SPD SMCサービルコール
 */
/*
 * [C-lang function] SETVCT: Set TOPPERS kernel entry points (fiq vector etc.)
 *
 * Argument :
 *   [In]  R0 - TOPPERS kernel entry points (fiq vector etc.)
 *
 * Return value:
 *   None
 */
    AALIGN(4)
    AGLOBAL(atf_smc_setvct)
ALABEL(atf_smc_setvct)
    /* SPD SMCサービルコール実行前のレジスタを保存 */
    save_atf_regs
    /* Execute SMC Function call */
    mov   r1, r0
    mov   r0, #0x0000   /* ATFSMC_SET_VECTOR */
    movt  r0, #((1 << 15) | (62 << 8))
    dsb
    smc   #0
    /* SPD SMCサービルコール実行前のレジスタを復帰 */
    restore_atf_regs
    /* 実行元へ戻る */
    mov   pc, lr

/*
 * [C-lang function] INIT:   Notify TOPPERS kernel initialize complete
 *
 * Argument :
 *   None
 *
 * Return value :
 *    0   Success
 *   -1   NS-EL1 OS isn't ready
 */
    AALIGN(4)
    AGLOBAL(atf_smc_init)
ALABEL(atf_smc_init)
    push  {lr}
    /* SPD SMCサービルコール実行前のレジスタを保存 */
    save_atf_regs
    /* Execute SMC Function call */
    ldr   r1, =atf_ns_dummy_entry
    mov   r0, #0x0001   /* ATFSMC_INIT_COMPLETE */
    movt  r0, #((1 << 15) | (62 << 8))
    dsb
    smc   #0
    /* 実行元へ戻る */
    pop   {lr}
    mov   pc, lr

/*
 * [C-lang function] CPUON:  Trun on a cpu core
 *
 * Argument :
 *   [In]  R0 - target cpu core (value of mpidr)
 *
 * Return value:
 *   None
 */
    AALIGN(4)
    AGLOBAL(atf_smc_cpuon)
ALABEL(atf_smc_cpuon)
    /* SPD SMCサービルコール実行前のレジスタを保存 */
    save_atf_regs
    /* Execute SMC Function call */
    mov   r3, #0
    ldr   r2, =atf_ns_dummy_entry
    mov   r1, r0
    mov   r0, #0x0002   /* ATFSMC_CPU_ON */
    movt  r0, #((1 << 15) | (62 << 8))
    dsb
    smc   #0
    /* SPD SMCサービルコール実行前のレジスタを復帰 */
    restore_atf_regs
    /* 実行元へ戻る */
    mov   pc, lr

/*
 * [C-lang function] SWITCH: Initiates a switch to the opposite world
 *
 * Argument :
 *   None
 *
 * Return value:
 *   0 - Success
 */
    AALIGN(4)
    AGLOBAL(atf_smc_switch)
ALABEL(atf_smc_switch)
    push  {lr}
    /* SPD SMCサービルコール実行前のレジスタを保存 */
    save_atf_regs
    /* Execute SMC Function call */
    mov   r0, #0x0003   /* ATFSMC_SWITCH_SECURE_STATE */
    movt  r0, #((1 << 15) | (62 << 8))
    dsb
    smc   #0
    /* 実行元へ戻る */
    pop   {lr}
    mov   pc, lr

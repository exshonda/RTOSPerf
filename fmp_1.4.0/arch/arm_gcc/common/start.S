/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: start.S 1210 2017-04-25 05:39:11Z ertl-honda $
 */

/*
 *   カーネル用のスタートアップモジュール(ARM用)
 */
#define    TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#include "kernel_impl.h"
#include "target_asm.inc"

/*
 *  同期用のマジックナンバー
 */
#define MAGIC_START 0x87654321

    ATEXT
    AALIGN(2)
    AGLOBAL(start)
ALABEL(start)
    /*
     *  プロセッサモードの初期化
     *  スーパーバイザーモード，FIQ/IRQ禁止
     */
    msr  cpsr_cxsf, #(CPSR_SVC AOR CPSR_INTLOCK)

    /*
     *  スタックポインタと，フレームポインタの初期化
     *  sl(スタック上限レジスタr10)は設定する必要があるか?
     *  gccはディフォルトでは-mapcsなので必要ない
     */
    my_istkpt sp, r0   /* FMP */
    mov  fp, #0        /* ARMモード用フレームポインタ(r11) */
    mov  r7, #0        /* Thumbモード用フレームポインタ    */

    /*
     *  hardware_init_hookの呼び出し （0でない場合）
     *
     *  ターゲットハードウェアに依存して必要な初期化処理がある場合
     *  は，hardware_init_hook という関数を用意する
     */
    ldr   r0, =hardware_init_hook
    cmp   r0, #0x00
    movne lr, pc
    bxne  r0

    /*
     *  マスタプロセッサ以外は初期化待ち
     */
    my_core_index r0
    cmp  r0, #(TOPPERS_MASTER_PRCID - 1)
    bne  slave_wait

    /*
     *  マスタプロセッサは変数を初期化
     */
    ldr  r0, =start_sync
    mov  r1, #0
    str  r1,[r0]

#ifndef TOPPERS_OMIT_BSS_INIT
    /*
     *  bssセクションのクリア
     */
ALABEL(start_1)
    ldr  r1, =BSS_START
    ldr  r2, =BSS_END
    cmp  r1,r2
    bhs  start_3
    mov  r0,#0
ALABEL(start_2)
    str  r0,[r1],#4
    cmp  r1, r2
    blo  start_2
#endif /* TOPPERS_OMIT_BSS_INIT */

ALABEL(start_3)
#ifndef TOPPERS_OMIT_DATA_INIT
    /*
     *  dataセクションの初期化(ROM化対応)
     *  IDATA_START から IDATA_END までを，DATA_START 以降に
     *  コピーする
     */
    ldr  r1, =IDATA_START
    ldr  r3, =IDATA_END
    cmp  r1, r3
    bhs  start_5
    ldr  r2, =DATA_START
ALABEL(start_4)
    ldr  r0, [r1],#4
    str  r0, [r2],#4
    cmp  r1, r3
    blo  start_4
#endif /* TOPPERS_OMIT_DATA_INIT */

    /*
     *  software_init_hook を呼出し（0 でない場合）
     *
     *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
     *  理がある場合は，software_init_hook という関数を用意すれば
     *  よい．
     */
ALABEL(start_5)
    ldr   r0, =software_init_hook
    cmp   r0, #0x00
    movne lr, pc
    bxne  r0

    /*
     *  target_mprc_initialize の呼び出し
     *
     *  sta_ker 以前にマスタプロセッサのみで行う必要のある初期化を実行
     */
ALABEL(start_6)
    bl   target_mprc_initialize

    /*
     *  カーネルを起動する
     */
ALABEL(start_7)
    ldr  r0, =start_sync
    ldr  r1, =MAGIC_START
    mov  r2, #0
ALABEL(start_8)
    str  r1, [r0, r2]
    add  r2, r2, #4
    cmp  r2, #(TNUM_PRCID * 4)
    bne  start_8

    /* マスタプロセッサ用のstart_syncを初期化 */
    my_core_index r1
    str  r2, [r0, r1, lsl #2]
    bl   sta_ker

    /*
     *  マスタプロセッサ以外の初期化待ちルーチン
     */
ALABEL(slave_wait)
    my_core_index r0
    ldr  r1, =start_sync
ALABEL(slave_wait_1)
    ldr  r2, =MAGIC_START
    ldr  r3, [r1, r0, lsl #2]
    cmp  r3, r2
    bne  slave_wait_1
    mov  r4, #0
    str  r4, [r1, r0, lsl #2]
    bl   sta_ker

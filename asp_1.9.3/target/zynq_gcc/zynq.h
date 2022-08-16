/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2006 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: zynq.h 2876 2019-09-29 05:23:51Z ertl-honda $
 */

#ifndef TOPPERS_ZYNQ_H
#define TOPPERS_ZYNQ_H

#include <sil.h>

/*
 * CPU clocks
 */
#ifdef ZYBO
#define ZYNQ_CPU_6X4X_MHZ		666U		/* 666MHz */
#define ZYNQ_CPU_3X2X_MHZ		333U		/* 333MHz */
#define ZYNQ_CPU_2X_MHZ			222U		/* 222MHz */
#define ZYNQ_CPU_1X_MHZ			111U		/* 111MHz */
#endif /* ZYBO */

#ifdef ZYBO_Z7
#define ZYNQ_CPU_6X4X_MHZ		667U		/* 667MHz */
#define ZYNQ_CPU_3X2X_MHZ		356U		/* 356MHz */
#define ZYNQ_CPU_2X_MHZ			222U		/* 222MHz */
#define ZYNQ_CPU_1X_MHZ			111U		/* 111MHz */
#endif /* ZYBO_Z7 */

#define CORE_CLK_MHZ  ZYNQ_CPU_6X4X_MHZ

#define    INHNO_SIO0  ZYNQ_UART1_IRQ
#define    INTNO_SIO0  ZYNQ_UART1_IRQ
#define    INTPRI_SIO0    -3
#define    INTATR_SIO0     0U


#define MPCORE_GTM_CLOCK_FREQ_MHZ  ZYNQ_CPU_3X2X_MHZ

/*
 * Tmer preescaler and load value to achieve 1ms per tick.
 * Note that the preescaler value must be representable with 8 bits;
 * and the load value must be a 32bit value.
 *
 * Private timers and watchdogs are clocked with PERIPHCLK which is 1/2 of
 * the CPU frequency. The formula is:
 *
 *    period = (preescaler + 1)(load + 1) / PERIPHCLK
 *      - period    = 1ms
 *      - periphclk = 325MHz
 *
 * For example:
 *      - preescaler = 0
 *      - load       = 325000
 */
#define MPCORE_TM_PS_1MS     0U       // preescaler for private timer
#define MPCORE_TM_LOAD_1MS   325000U  // load value for private timer


/*
 * Definition of the 115200bps baud rate constants
 *        rate = clk / (CD * (BDIV + 1))
 *         clk = MR.CLKSEL? inclk : inclk/8  (e.g. inclk)
 *       inclk = MR.CCLK? uart_clock : apb clock (e.g. uart_clock=50MHz)
 */
#define UART_BAUD_115K          0x7CU
#define UART_BAUDDIV_115K       0x6U

/*
 *  UART base address definitions (used in target_serial.c)
 */
#define     UART0_BASE  ZYNQ_UART1_BASE
#define     UART1_BASE  ZYNQ_UART0_BASE

/*
 *  Common Definition
 */
  
/*
 * Available UARTs
 */
#define ZYNQ_UART0_BASE 0xE0000000
#define ZYNQ_UART1_BASE 0xE0001000
#define ZYNQ_UART0_IRQ  (59)
#define ZYNQ_UART1_IRQ  (82)

/*
 *  L2(PL310) Base Address
 */
#define PL310_L2CACHE_BASE	0xF8F02000

/*
 *  MPCore Private Memory Regin Base Address
 */
#define MPCORE_PMR_BASE  0xF8F00000


#define GIC_TMAX_INTNO   95U
#define GIC_TNUM_INT     96U


#endif /* TOPPERS_ZYNQ_H */

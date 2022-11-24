/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2007,2011,2015 by Embedded and Real-Time Systems Laboratory
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
 */

#ifndef TOPPERS_RP2040_H
#define TOPPERS_RP2040_H

#define RP2040_CLOCKS_BASE             0x40008000
#define RP2040_CLOCKS_CLK_REF_CTRL     (uint32_t *)(RP2040_CLOCKS_BASE + 0x30)
#define RP2040_CLOCKS_CLK_REF_DIV      (uint32_t *)(RP2040_CLOCKS_BASE + 0x34)
#define RP2040_CLOCKS_CLK_REF_SELECTED (uint32_t *)(RP2040_CLOCKS_BASE + 0x38)
#define RP2040_CLOCKS_CLK_SYS_CTRL     (uint32_t *)(RP2040_CLOCKS_BASE + 0x3C)
#define RP2040_CLOCKS_CLK_SYS_DIV      (uint32_t *)(RP2040_CLOCKS_BASE + 0x40)
#define RP2040_CLOCKS_CLK_SYS_SELECTED (uint32_t *)(RP2040_CLOCKS_BASE + 0x44)
#define RP2040_CLOCKS_CLK_PERI_CTRL    (uint32_t *)(RP2040_CLOCKS_BASE + 0x48)
#define RP2040_CLOCKS_CLK_SYS_RESUS_CTRL (uint32_t *)(RP2040_CLOCKS_BASE + 0x78)

#define RP2040_CLOCKS_CLK_REF_CTRL_SRC_XOSC       (0x2 << 0)
#define RP2040_CLOCKS_CLK_REF_CTRL_SRC_BITS       (0x3 << 0)
#define RP2040_CLOCKS_CLK_SYS_CTRL_AUXSRC_PLL_SYS (0x0 << 5)
#define RP2040_CLOCKS_CLK_SYS_CTRL_SRC_REF        (0x0 << 0)
#define RP2040_CLOCKS_CLK_SYS_CTRL_SRC_AUX        (0x1 << 0)
#define RP2040_CLOCKS_CLK_SYS_CTRL_SRC_BIT        (0x1 << 0)
#define RP2040_CLOCKS_CLK_PERI_CTRL_ENABLE        (1 << 11)
#define RP2040_CLOCKS_CLK_PERI_CTRL_SRC_CLK_SYS   (1 << 0)

#define RP2040_RESETS_BASE        0x4000c000
#define RP2040_RESETS_RESET       (uint32_t *)(0x4000c000 + 0x0)
#define RP2040_RESETS_RESET_DONE  (uint32_t *)(0x4000c000 + 0x8)

#define RP2040_RESETS_RESET_UART1     (1 << 23)
#define RP2040_RESETS_RESET_UART0     (1 << 22)
#define RP2040_RESETS_RESET_TIMER     (1 << 21)
#define RP2040_RESETS_RESET_PLL_SYS   (1 << 12)
#define RP2040_RESETS_RESET_PADS_QSPI (1 << 9)
#define RP2040_RESETS_RESET_IO_QSPI   (1 << 6)
#define RP2040_RESETS_RESET_IO_BANK0  (1 << 5)

#define RP2040_IO_BANK0_BASE           0x40014000
#define RP2040_IO_BANK0_GPIO_STATUS(n) (uint32_t *)(RP2040_IO_BANK0_BASE + (n) * 8)
#define RP2040_IO_BANK0_GPIO_CTRL(n)   (uint32_t *)(RP2040_IO_BANK0_BASE + (n) * 8 + 4)

#define RP2040_XOSC_BASE    0x40024000
#define RP2040_XOSC_CTRL    (uint32_t *)(RP2040_XOSC_BASE + 0x00)
#define RP2040_XOSC_STATUS  (uint32_t *)(RP2040_XOSC_BASE + 0x04)
#define RP2040_XOSC_DORMANT (uint32_t *)(RP2040_XOSC_BASE + 0x08)
#define RP2040_XOSC_STARTUP (uint32_t *)(RP2040_XOSC_BASE + 0x0C)
#define RP2040_XOSC_COUNT   (uint32_t *)(RP2040_XOSC_BASE + 0x1C)

#define RP2040_XOSC_CTRL_ENABLE_MAGIC     (0xfab << 12)
#define RP2040_XOSC_CTRL_FREQ_RANGE_MAGIC (0xaa0 << 0)
#define RP2040_XOSC_STATUS_STABLE         (1 << 31)

#define RP2040_PLL_SYS_BASE      0x40028000
#define RP2040_PLL_SYS_CS        (uint32_t *)(RP2040_PLL_SYS_BASE + 0x00)
#define RP2040_PLL_SYS_PWR       (uint32_t *)(RP2040_PLL_SYS_BASE + 0x04)
#define RP2040_PLL_SYS_FBDIV_INT (uint32_t *)(RP2040_PLL_SYS_BASE + 0x08)
#define RP2040_PLL_SYS_PRIM      (uint32_t *)(RP2040_PLL_SYS_BASE + 0x0C)

#define RP2040_PLL_SYS_CS_LOCK          (1 << 31)
#define RP2040_PLL_SYS_CS_REFDIV(n)     ((n) << 0)
#define RP2040_PLL_SYS_PRIM_POSTDIV1(n) ((n) << 16)
#define RP2040_PLL_SYS_PWR_PD           (1 << 0)
#define RP2040_PLL_SYS_PWR_VCOPD        (1 << 5)
#define RP2040_PLL_SYS_PRIM_POSTDIV2(n) ((n) << 12)

#define RP2040_UART0_BASE       0x40034000
#define RP2040_UART1_BASE       0x40038000
#define RP2040_UART_DR(base)    (uint32_t *)((base) + 0x000)
#define RP2040_UART_FR(base)    (uint32_t *)((base) + 0x018)
#define RP2040_UART_IBRD(base)  (uint32_t *)((base) + 0x024)
#define RP2040_UART_FBRD(base)  (uint32_t *)((base) + 0x028)
#define RP2040_UART_LCR_H(base) (uint32_t *)((base) + 0x02C)
#define RP2040_UART_CR(base)    (uint32_t *)((base) + 0x030)
#define RP2040_UART_IFLS(base)  (uint32_t *)((base) + 0x034)
#define RP2040_UART_IMSC(base)  (uint32_t *)((base) + 0x038)
#define RP2040_UART_MIS(base)   (uint32_t *)((base) + 0x040)
#define RP2040_UART_ICR(base)   (uint32_t *)((base) + 0x044)

#define RP2040_UART_FR_TXFF          (1 << 5)
#define RP2040_UART_FR_RXFE          (1 << 4)
#define RP2040_UART_LCR_H_WLEN_8BITS (0b11 << 5)
#define RP2040_UART_LCR_H_FEN        (1 << 4)
#define RP2040_UART_CR_RXE           (1 << 9)
#define RP2040_UART_CR_TXE           (1 << 8)
#define RP2040_UART_CR_UARTEN        (1 << 0)
#define RP2040_UART_IMSC_RTIM        (1 << 6)
#define RP2040_UART_IMSC_TXIM        (1 << 5)
#define RP2040_UART_IMSC_RXIM        (1 << 4)
#define RP2040_UART_MIS_RTIM         (1 << 6)
#define RP2040_UART_MIS_TXMIS        (1 << 5)
#define RP2040_UART_MIS_RXMIS        (1 << 4)
#define RP2040_UART_ICR_RTIM         (1 << 6)
#define RP2040_UART_ICR_TXIC         (1 << 5)
#define RP2040_UART_ICR_RXIC         (1 << 4)

#define RP2040_WATCHDOG_BASE 0x40058000
#define RP2040_WATCHDOG_TICK (uint32_t *)(RP2040_WATCHDOG_BASE + 0x2c)

#define RP2040_WATCHDOG_RUNNING (1 << 10)
#define RP2040_WATCHDOG_ENABLE  (1 << 9)

#define RP2040_TIMER_BASE     0x40054000
#define RP2040_TIMER_ALARM0   (uint32_t *)(RP2040_TIMER_BASE + 0x10)
#define RP2040_TIMER_ARMED    (uint32_t *)(RP2040_TIMER_BASE + 0x20)
#define RP2040_TIMER_TIMERAWL (uint32_t *)(RP2040_TIMER_BASE + 0x28)
#define RP2040_TIMER_INTR     (uint32_t *)(RP2040_TIMER_BASE + 0x34)
#define RP2040_TIMER_INTE     (uint32_t *)(RP2040_TIMER_BASE + 0x38)
#define RP2040_TIMER_INTF     (uint32_t *)(RP2040_TIMER_BASE + 0x3C)

#define RP2040_TIMER_INT_ALARM_0 (1 << 0)

#define RP2040_TIMER0_IRQn 0
#define RP2040_TIMER1_IRQn 1
#define RP2040_TIMER2_IRQn 2
#define RP2040_TIMER3_IRQn 3
#define RP2040_UART0_IRQn  20
#define RP2040_UART1_IRQn  21

#endif /* TOPPERS_RP2040_H */

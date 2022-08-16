/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.c 2828 2017-07-06 13:28:56Z ertl-honda $
 */

/*
 * ターゲット依存モジュール（ZYNQ用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "pl310.h"

/*
 *  System log low-level (polling) character output
 */
extern void xuartps_pol_putc(char c);

void
target_fput_log(char c)
{
	if (c == '\n') {
		xuartps_pol_putc('\r');
	}
	xuartps_pol_putc(c);
}

/*
 * MMU initialization (called from the chip code)
 *
 * We create a simpe 1-to-1 (physical address = virtual address) memory
 * mapping and enable caching for the DDR memory.
 *
 * Note: when not using SafeG the default world is Secure. When using
 * SafeG, Secure world uses secure accesses by default. If you want to
 * access a shared memory region with non-secure access, you will have
 * to configure it manually.
 */
void
target_mmu_init(void)
{
	MEMORY_ATTRIBUTE m_attribute;

	/*
	 * Basic initialization of the whole virtual memory space to
	 * non-cachable, non-bufferable, strongly-ordered.
	 */
	m_attribute.pa   = 0x00000000;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = 0x80000000; /* 2GB */
	m_attribute.ns   = 0;     /* 0=Secure */
	m_attribute.s    = 1;     /* 1=Shared */
	m_attribute.ap   = 3;     /* Full access */
	m_attribute.tex  = 0;     /* Strongly Ordered */
	m_attribute.c    = 0;
	m_attribute.b    = 0;
	mmu_map_memory (&m_attribute);

	m_attribute.pa   = 0x80000000; /* 2GB */
	m_attribute.va   = m_attribute.pa;
	mmu_map_memory (&m_attribute);

	/*
	 *  Enable caching for the DDR
	 */
	m_attribute.pa   = 0;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = 0x20000000;
	m_attribute.ns   = 0;      /* 0=NonSecure */
	m_attribute.s    = 1;      /* 1=Shared    */
	m_attribute.ap   = 3;      /* Full access */
	m_attribute.tex  = 1;
	m_attribute.c    = 1;             
	m_attribute.b    = 1;             

	mmu_map_memory(&m_attribute);
}

/*
 *  OS起動時の初期化
 */
void
hardware_init_hook1(void)
{
	dcache_disable();
	pl310_invalidate_all();
	icache_disable();
}

/*
 *  Target-dependent initialization
 */
extern void xuartps_init();

void
target_initialize(void)
{
	extern void	*vector_table;		/* ベクタテーブル */

	hardware_init_hook1();

	/*
	 * Initialize chip
	 */
	chip_initialize();

	/*
	 *  ベクタテーブルの設定
	 */
	CP15_SET_VBAR((uint32_t) &vector_table);

	/*
	 *  L2キャッシュコントローラ（PL310）の初期化
	 */
	pl310_init(0, ~0x0U);

	/*
	 *  UARTを初期化
	 */
	xuartps_init();
}

/*
 *  Target-dependent exit processing
 */
void
target_exit(void)
{
	/*
	 *  Chip exit processing
	 */
	chip_exit();

	/*
	 *  Endless loop
	 */
	while(1);
}

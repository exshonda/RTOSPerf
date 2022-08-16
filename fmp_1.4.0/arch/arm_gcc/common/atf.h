/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2018-2019 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id $
 */

#ifndef TOPPERS_ATF_H
#define TOPPERS_ATF_H

/*
 *  Bit definitions inside the function id as per the SMC calling convention
 */
#define FUNCID_TYPE_SHIFT	31
#define FUNCID_CC_SHIFT		30
#define FUNCID_OEN_SHIFT	24
#define FUNCID_NUM_SHIFT	0

#define FUNCID_TYPE_MASK	0x1
#define FUNCID_CC_MASK		0x1
#define FUNCID_OEN_MASK		0x3f
#define FUNCID_NUM_MASK		0xffff

#define FUNCID_TYPE_WIDTH	1
#define FUNCID_CC_WIDTH		1
#define FUNCID_OEN_WIDTH	6
#define FUNCID_NUM_WIDTH	16

#define GET_SMC_CC(id)      ((id >> FUNCID_CC_SHIFT) & \
                     FUNCID_CC_MASK)
#define GET_SMC_TYPE(id)    ((id >> FUNCID_TYPE_SHIFT) & \
                     FUNCID_TYPE_MASK)

#define SMC_64				1U
#define SMC_32				0U
#define SMC_UNK				0xffffffff
#define SMC_TYPE_FAST		1U
#define SMC_TYPE_STD		0U
#define SMC_PREEMPTED		0xfffffffe


/*
 *  SMC Function call IDs
 */
#define ATFSMC_RV(func_num) \
		((SMC_TYPE_FAST << FUNCID_TYPE_SHIFT) | \
		((SMC_32) << FUNCID_CC_SHIFT) | \
		(62 << FUNCID_OEN_SHIFT) | \
		((func_num) & FUNCID_NUM_MASK))

/* SETVCT: Set TOPPERS kernel entry points (fiq vector etc.) */
#define ATFSMC_SET_VECTOR			ATFSMC_RV(0)
/* INIT:   Notification TOPPERS/FMP initialize complete */
#define ATFSMC_INIT_COMPLETE		ATFSMC_RV(1)
/* CPUON:  Issued when returning from "cpu_on" vector */
#define ATFSMC_SMC_CPU_ON			ATFSMC_RV(2)
/* SWITCH: initiates a switch to the opposite world */
#define ATFSMC_SWITCH_SECURE_STATE	ATFSMC_RV(3)
/* GENINT: Generate secure interrupt */
#define ATFSMC_GEN_INT				ATFSMC_RV(4)

/*
 *  SMC Function call return codes
 */
#define	ATFSMC_RET_SUCCESS			0
#define	ATFSMC_RET_INVALID_STATE	-1
#define	ATFSMC_RET_PARAM_ERROR		-2
#define	ATFSMC_RET_NOT_READY		-3

/*
 *  Type defination
 */
/* Information of entry points */
typedef uint32_t		vector_t;
typedef int32_t			atf_ret_t;

typedef struct
{
	vector_t	cpu_on_entry;
	vector_t	fiq_entry;
} atfsmc_vectors_t;

/*
 *  Proto types
 */
/* ATF Trusted OS Vector Table */
extern atfsmc_vectors_t atf_vector_table;
/* SETVCT: Set TOPPERS kernel entry points (fiq vector etc.) */
extern atf_ret_t atf_smc_setvct(atfsmc_vectors_t *vectors);
/* INIT: Notify TOPPERS kernel initialize complete */
extern atf_ret_t atf_smc_init(void);
/* CPUON:    Trun on a cpu core */
extern atf_ret_t atf_smc_cpuon(uint64_t target_cpu);
/* SWITCH: Initiates a switch to the opposite world */
extern atf_ret_t atf_smc_switch(void);

#endif  /* TOPPERS_ATF_H */

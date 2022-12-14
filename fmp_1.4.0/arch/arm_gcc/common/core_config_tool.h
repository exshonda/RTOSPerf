/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2017 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: core_config_tool.h 1270 2018-07-12 04:25:51Z ertl-honda $
 */

/*
 *  チップ依存モジュールのツール依存部（ARM用）
 *
 *  カーネルのチップ依存部のインクルードファイルのツール依存部．
 *  chip_config.h からインクルードされる．
 */

#ifndef TOPPERS_CORE_CONFIG_TOOL_H
#define TOPPERS_CORE_CONFIG_TOOL_H

#ifndef TOPPERS_MACRO_ONLY

#if __TARGET_ARCH_ARM >= 6

#ifndef USE_GIC_CPULOCK
/*
 *  ロック取得解放関数
 *  x_lock_cpu()/x_unlock_cpu()を使うため，コア依存モジュール（ARM用）を
 *  読み込んでから定義する．
 */

/*
 *  ロック取得失敗時の割込み待ち
 *
 *  ロック取得失敗時にスピンを繰り返すのではなく，wfeにより，
 *  イベント待ちとすることが推奨されている．
 *  wfeによるイベント待ちの際，割込み応答性を低下させないため，割込みを許
 *  可した状態て，イベント待ちとするべきである．wfe命令自信は，割込みを禁
 *  止・許可を操作しないため，別途msr命令により割込みを許可する．
 *
 *  その際，msr実行により割込みが許可された直後(wfeを実行する前に)に割込
 *  みが入り，その後割込みハンドラからのリターン後にwfeになり，イベントを
 *  逃す可能性が考えられる．
 *
 *  しかしながら， 割込みハンドラからのリターン(正確には例外一般からのリタ
 *  ーン)を行うと，イベントレジスタがセットされ，wfe実行時にイベントレジス
 *  タがセットされていると，クリアしてwfeから即座にリターンするための問題
 *  ない．
 */

/*
 *  1段目のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t msrne    cpsr_c, %2  \n" /* 割込みの許可(bit0-7のみ書き換え) */
			#ifndef TOPPERS_OMIT_USE_WFE
			"\t wfene                \n"
			#endif /* TOPPERS_OMIT_USE_WFE */
			"\t msrne    cpsr_c, %3  \n" /* 割込みの禁止(bit0-7のみ書き換え) */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock),"I"(CPSR_SVC|CPSR_ALWAYS_SET),"I"(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
			:"r1","r2", "cc");

		if(locked == 0){
			/* ロック取得成功 */
			CP15_DATA_MEMORY_BARRIER();
			Asm("":::"memory");
			return;
		}
	}
}

#define t_acquire_lock(p_lock) x_acquire_lock(p_lock)
#define i_acquire_lock(p_lock) x_acquire_lock(p_lock)

#if TTYPE_KLOCK != G_KLOCK

/*
 *  2段目以上のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline bool_t
x_acquire_nested_lock(LOCK *p_lock)
{
	PCB		*p_pcb;
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t msrne    cpsr_c, %2  \n" /* 割込みの許可(bit0-7のみ書き換え) */
			#ifndef TOPPERS_OMIT_USE_WFE
			"\t wfene                \n"
			#endif /* TOPPERS_OMIT_USE_WFE */
			"\t msrne    cpsr_c, %3  \n" /* 割込みの禁止(bit0-7のみ書き換え) */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock),"I"(CPSR_SVC|CPSR_ALWAYS_SET),"I"(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
			:"r1","r2", "cc");
		if(locked == 0){
			/* ロック取得成功 */
			CP15_DATA_MEMORY_BARRIER();
			Asm("":::"memory");
			return(false);
		}
		/*
		 * マイグレーションする可能性があるのでここで毎回取得
		 * 非タスクコンテキストの場合，マイグレーションしないため，
		 * while前に実行してもよいが，1回でロックがとれた場合，
		 * 効率が悪いので，ここで取得する．
		 */
		p_pcb = get_my_p_pcb();
		if (p_pcb->p_firstlock == NULL) {
			return(true);
		}
	}
}

#define t_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)
#define i_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)

#endif /* TTYPE_KLOCK != G_KLOCK */

/*
 *  ロックの解放（タスク・非タスクコンテキスト共用）
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	Asm("":::"memory");
	CP15_DATA_MEMORY_BARRIER();
	*p_lock = 0;
	CP15_DATA_SYNC_BARRIER();
	Asm("sev");
}

/*
 *  割込を許可しないロック取得関数
 */
Inline void
x_acquire_lock_without_preemption(LOCK *p_lock)
{
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			#ifndef TOPPERS_OMIT_USE_WFE
			"\t wfene                \n"
			#endif /* TOPPERS_OMIT_USE_WFE */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked):"r"(p_lock):"r1","r2", "cc");
		if(locked == 0){
			/* ロック取得成功 */
			CP15_DATA_MEMORY_BARRIER();
			Asm("":::"memory");
			return;
		}
	}
}

#else /* USE_GIC_CPULOCK */

/*
 *  ロック取得解放関数（USE_GIC_CPULOCK時）
 *  x_lock_cpu()/x_unlock_cpu()を使うため，コア依存モジュール（ARM用）を
 *  読み込んでから定義する．
 */

/*
 *  ロック取得失敗時の割込み待ち
 *
 *  ロック取得失敗時にスピンを繰り返すのではなく，wfeにより，
 *  イベント待ちとすることが推奨されている．
 *  wfeによるイベント待ちの際，割込み応答性を低下させないため，割込みを許
 *  可した状態て，イベント待ちとするべきである．wfe命令自信は，割込みを禁
 *  止・許可を操作しないため，別途msr命令により割込みを許可する．
 *
 *  その際，msr実行により割込みが許可された直後(wfeを実行する前に)に割込
 *  みが入り，その後割込みハンドラからのリターン後にwfeになり，イベントを
 *  逃す可能性が考えられる．
 *
 *  しかしながら， 割込みハンドラからのリターン(正確には例外一般からのリタ
 *  ーン)を行うと，イベントレジスタがセットされ，wfe実行時にイベントレジス
 *  タがセットされていると，クリアしてwfeから即座にリターンするための問題
 *  ない．
 */

/*
 *  1段目のロック取得（タスク・非タスクコンテキスト共用）（USE_GIC_CPULOCK時）
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	int		locked;
	uint8_t iipm;

	iipm = gicc_get_priority() >> GIC_PRI_SHIFT;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock)
			:"r1","r2", "cc");

		if(locked == 0){
			/* ロック取得成功 */
			CP15_DATA_MEMORY_BARRIER();
			Asm("":::"memory");
			return;
		}
		else {
			/* 一旦割込みを許可にする */
			gicc_set_priority((GIC_PRI_LEVEL - 1) << GIC_PRI_SHIFT);
			gicc_set_priority(iipm << GIC_PRI_SHIFT);
		}
	}
}

#define t_acquire_lock(p_lock) x_acquire_lock(p_lock)
#define i_acquire_lock(p_lock) x_acquire_lock(p_lock)

#if TTYPE_KLOCK != G_KLOCK

/*
 *  2段目以上のロック取得（タスク・非タスクコンテキスト共用）（USE_GIC_CPULOCK時）
 */
Inline bool_t
x_acquire_nested_lock(LOCK *p_lock)
{
	PCB		*p_pcb;
	int		locked;
	uint8_t iipm;

	iipm = gicc_get_priority() >> GIC_PRI_SHIFT;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock)
			:"r1","r2", "cc");
		if(locked == 0){
			/* ロック取得成功 */
			CP15_DATA_MEMORY_BARRIER();
			Asm("":::"memory");
			return(false);
		}else{
			/* 一旦割込みを許可にする */
			gicc_set_priority((GIC_PRI_LEVEL - 1) << GIC_PRI_SHIFT);
			gicc_set_priority(iipm << GIC_PRI_SHIFT);
		}

		/*
		 * マイグレーションする可能性があるのでここで毎回取得
		 * 非タスクコンテキストの場合，マイグレーションしないため，
		 * while前に実行してもよいが，1回でロックがとれた場合，
		 * 効率が悪いので，ここで取得する．
		 */
		p_pcb = get_my_p_pcb();
		if (p_pcb->p_firstlock == NULL) {
			return(true);
		}
	}
}

#define t_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)
#define i_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)

#endif /* TTYPE_KLOCK != G_KLOCK */

/*
 *  ロックの解放（タスク・非タスクコンテキスト共用）（USE_GIC_CPULOCK時）
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	Asm("":::"memory");
	CP15_DATA_MEMORY_BARRIER();
	*p_lock = 0;
	CP15_DATA_SYNC_BARRIER();
	Asm("sev");
}

/*
 *  割込を許可しないロック取得関数（USE_GIC_CPULOCK時）
 */
Inline void
x_acquire_lock_without_preemption(LOCK *p_lock)
{
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked):"r"(p_lock):"r1","r2", "cc");
		if(locked == 0){
			/* ロック取得成功 */
			CP15_DATA_MEMORY_BARRIER();
			Asm("":::"memory");
			return;
		}
	}
}

#endif /* USE_GIC_CPULOCK */ 

/*
 *  スピンロック機能のネイティブロック方式で使用する関数
 */

/*
 *  スピンロックの型
 */
typedef uint32_t SPNLOCK;

/*
 *  スピンロックの初期化
 */
Inline void
x_initialize_spin(ID spnid, SPNLOCK *p_spn_lock)
{
	*p_spn_lock = 0;
}

/*
 *  スピンロックの取得
 */
Inline void
x_lock_spin(SPNLOCK *p_spn_lock)
{
	x_acquire_lock(p_spn_lock);
}

#define t_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)
#define i_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)

/*
 *  スピンロックの取得の試行
 */
Inline bool_t
x_try_lock_spin(SPNLOCK *p_spn_lock)
{
	int locked;

	Asm("   mov      r2, #0x01   \n"
		"\t ldrex    r1, [%1]    \n"
		"\t cmp      r1, #0x00   \n"
		"\t strexeq  r1, r2,[%1] \n"
		"\t mov      %0, r1      \n"
		:"=r"(locked):"r"(p_spn_lock):"r1","r2", "cc");

	if (locked == 0) {
		/* 成功した場合 */
		CP15_DATA_MEMORY_BARRIER();
		Asm("":::"memory");
		return(false);
	}

	return(true);
}

/*
 *  スピンロックの返却
 */
Inline void
x_unlock_spin(SPNLOCK *p_spn_lock)
{
	x_release_lock(p_spn_lock);
}
#endif /* __TARGET_ARCH_ARM >= 6 */
#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_CORE_CONFIG_TOOL_H */

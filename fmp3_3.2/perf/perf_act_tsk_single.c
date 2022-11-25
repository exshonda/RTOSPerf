/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2008-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  荳願ｨ倩送菴懈ｨｩ閠�縺ｯ�ｼ御ｻ･荳九�ｮ(1)�ｽ�(4)縺ｮ譚｡莉ｶ繧呈ｺ�縺溘☆蝣ｴ蜷医↓髯舌ｊ�ｼ梧悽繧ｽ繝輔ヨ繧ｦ繧ｧ
 *  繧｢�ｼ域悽繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢繧呈隼螟峨＠縺溘ｂ縺ｮ繧貞性繧��ｼ惹ｻ･荳句酔縺假ｼ峨ｒ菴ｿ逕ｨ繝ｻ隍�陬ｽ繝ｻ謾ｹ
 *  螟峨�ｻ蜀埼�榊ｸ�ｼ井ｻ･荳具ｼ悟茜逕ｨ縺ｨ蜻ｼ縺ｶ�ｼ峨☆繧九％縺ｨ繧堤┌蜆溘〒險ｱ隲ｾ縺吶ｋ�ｼ�
 *  (1) 譛ｬ繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢繧偵た繝ｼ繧ｹ繧ｳ繝ｼ繝峨�ｮ蠖｢縺ｧ蛻ｩ逕ｨ縺吶ｋ蝣ｴ蜷医↓縺ｯ�ｼ御ｸ願ｨ倥�ｮ闡嶺ｽ�
 *      讓ｩ陦ｨ遉ｺ�ｼ後％縺ｮ蛻ｩ逕ｨ譚｡莉ｶ縺翫ｈ縺ｳ荳玖ｨ倥�ｮ辟｡菫晁ｨｼ隕丞ｮ壹′�ｼ後◎縺ｮ縺ｾ縺ｾ縺ｮ蠖｢縺ｧ繧ｽ繝ｼ
 *      繧ｹ繧ｳ繝ｼ繝我ｸｭ縺ｫ蜷ｫ縺ｾ繧後※縺�繧九％縺ｨ�ｼ�
 *  (2) 譛ｬ繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢繧抵ｼ後Λ繧､繝悶Λ繝ｪ蠖｢蠑上↑縺ｩ�ｼ御ｻ悶�ｮ繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢髢狗匱縺ｫ菴ｿ
 *      逕ｨ縺ｧ縺阪ｋ蠖｢縺ｧ蜀埼�榊ｸ�縺吶ｋ蝣ｴ蜷医↓縺ｯ�ｼ悟�埼�榊ｸ�縺ｫ莨ｴ縺�繝峨く繝･繝｡繝ｳ繝茨ｼ亥茜逕ｨ
 *      閠�繝槭ル繝･繧｢繝ｫ縺ｪ縺ｩ�ｼ峨↓�ｼ御ｸ願ｨ倥�ｮ闡嶺ｽ懈ｨｩ陦ｨ遉ｺ�ｼ後％縺ｮ蛻ｩ逕ｨ譚｡莉ｶ縺翫ｈ縺ｳ荳玖ｨ�
 *      縺ｮ辟｡菫晁ｨｼ隕丞ｮ壹ｒ謗ｲ霈峨☆繧九％縺ｨ�ｼ�
 *  (3) 譛ｬ繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢繧抵ｼ梧ｩ溷勣縺ｫ邨�縺ｿ霎ｼ繧�縺ｪ縺ｩ�ｼ御ｻ悶�ｮ繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢髢狗匱縺ｫ菴ｿ
 *      逕ｨ縺ｧ縺阪↑縺�蠖｢縺ｧ蜀埼�榊ｸ�縺吶ｋ蝣ｴ蜷医↓縺ｯ�ｼ梧ｬ｡縺ｮ縺�縺壹ｌ縺九�ｮ譚｡莉ｶ繧呈ｺ�縺溘☆縺�
 *      縺ｨ�ｼ�
 *    (a) 蜀埼�榊ｸ�縺ｫ莨ｴ縺�繝峨く繝･繝｡繝ｳ繝茨ｼ亥茜逕ｨ閠�繝槭ル繝･繧｢繝ｫ縺ｪ縺ｩ�ｼ峨↓�ｼ御ｸ願ｨ倥�ｮ闡�
 *        菴懈ｨｩ陦ｨ遉ｺ�ｼ後％縺ｮ蛻ｩ逕ｨ譚｡莉ｶ縺翫ｈ縺ｳ荳玖ｨ倥�ｮ辟｡菫晁ｨｼ隕丞ｮ壹ｒ謗ｲ霈峨☆繧九％縺ｨ�ｼ�
 *    (b) 蜀埼�榊ｸ�縺ｮ蠖｢諷九ｒ�ｼ悟挨縺ｫ螳壹ａ繧区婿豕輔↓繧医▲縺ｦ�ｼ卦OPPERS繝励Ο繧ｸ繧ｧ繧ｯ繝医↓
 *        蝣ｱ蜻翫☆繧九％縺ｨ�ｼ�
 *  (4) 譛ｬ繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢縺ｮ蛻ｩ逕ｨ縺ｫ繧医ｊ逶ｴ謗･逧�縺ｾ縺溘�ｯ髢捺磁逧�縺ｫ逕溘§繧九＞縺九↑繧区錐
 *      螳ｳ縺九ｉ繧ゑｼ御ｸ願ｨ倩送菴懈ｨｩ閠�縺翫ｈ縺ｳTOPPERS繝励Ο繧ｸ繧ｧ繧ｯ繝医ｒ蜈崎ｲｬ縺吶ｋ縺薙→�ｼ�
 *      縺ｾ縺滂ｼ梧悽繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢縺ｮ繝ｦ繝ｼ繧ｶ縺ｾ縺溘�ｯ繧ｨ繝ｳ繝峨Θ繝ｼ繧ｶ縺九ｉ縺ｮ縺�縺九↑繧狗炊
 *      逕ｱ縺ｫ蝓ｺ縺･縺剰ｫ区ｱゅ°繧峨ｂ�ｼ御ｸ願ｨ倩送菴懈ｨｩ閠�縺翫ｈ縺ｳTOPPERS繝励Ο繧ｸ繧ｧ繧ｯ繝医ｒ
 *      蜈崎ｲｬ縺吶ｋ縺薙→�ｼ�
 *
 *  譛ｬ繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢縺ｯ�ｼ檎┌菫晁ｨｼ縺ｧ謠蝉ｾ帙＆繧後※縺�繧九ｂ縺ｮ縺ｧ縺ゅｋ�ｼ惹ｸ願ｨ倩送菴懈ｨｩ閠�縺�
 *  繧医�ｳTOPPERS繝励Ο繧ｸ繧ｧ繧ｯ繝医�ｯ�ｼ梧悽繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢縺ｫ髢｢縺励※�ｼ檎音螳壹�ｮ菴ｿ逕ｨ逶ｮ逧�
 *  縺ｫ蟇ｾ縺吶ｋ驕ｩ蜷域�ｧ繧ょ性繧√※�ｼ後＞縺九↑繧倶ｿ晁ｨｼ繧り｡後ｏ縺ｪ縺�ｼ弱∪縺滂ｼ梧悽繧ｽ繝輔ヨ繧ｦ繧ｧ
 *  繧｢縺ｮ蛻ｩ逕ｨ縺ｫ繧医ｊ逶ｴ謗･逧�縺ｾ縺溘�ｯ髢捺磁逧�縺ｫ逕溘§縺溘＞縺九↑繧区錐螳ｳ縺ｫ髢｢縺励※繧ゑｼ後◎
 *  縺ｮ雋ｬ莉ｻ繧定ｲ�繧上↑縺�ｼ�
 *
 *  @(#) $Id: perf_act_tsk.c 1210 2017-04-25 05:39:11Z ertl-honda $
 */

/*
 *  act_tsk 諤ｧ閭ｽ貂ｬ螳壹�励Ο繧ｰ繝ｩ繝�
 */

#include <kernel.h>
#include <t_syslog.h>
#include <sil.h>
#include "syssvc/syslog.h"
#include "syssvc/histogram.h"
#include "kernel_cfg.h"
#include "perf_act_tsk_single.h"
#include "target_test.h"

/*
 *  險域ｸｬ蝗樊焚縺ｨ螳溯｡梧凾髢灘�蟶�繧定ｨ倬鹸縺吶ｋ譛�螟ｧ譎る俣
 */
#define NO_MEASURE	20000U			/* 險域ｸｬ蝗樊焚 */
#define MAX_TIME	400000U			/* 螳溯｡梧凾髢灘�蟶�繧定ｨ倬鹸縺吶ｋ譛�螟ｧ譎る俣 */

/*
 *  險域ｸｬ縺ｮ蜑榊ｾ後〒縺ｮ繝輔ャ繧ｯ繝ｫ繝ｼ繝√Φ
 */
#ifndef CPU1_PERF_PRE_HOOK
#define CPU1_PERF_PRE_HOOK
#endif  /* CPU1_PERF_PRE_HOOK */
#ifndef CPU1_PERF_POST_HOOK
#define CPU1_PERF_POST_HOOK
#endif  /* CPU1_PERF_POST_HOOK */
#ifndef CPU2_PERF_PRE_HOOK
#define CPU2_PERF_PRE_HOOK
#endif  /* CPU2_PERF_PRE_HOOK */
#ifndef CPU2_PERF_POST_HOOK
#define CPU2_PERF_POST_HOOK
#endif  /* CPU2_PERF_POST_HOOK */

void task1_1(intptr_t exinf)
{
	end_measure(1);
}


void task1_2(intptr_t exinf)
{
	;
}


void task1_3(intptr_t exinf)
{
	slp_tsk();
}


/*
 * 險域ｸｬ繝ｫ繝ｼ繝√Φ
 */
void perf_eval(uint_t n)
{
	uint_t	i;

	init_hist(1);
	syslog_fls_log();
	dly_tsk(1000000);
#error
	CPU1_PERF_PRE_HOOK;

	for ( i = 0; i < NO_MEASURE; i++ ) {
		switch (n) {
			//繧ｪ繝ｼ繝舌�ｼ繝倥ャ繝峨�ｮ貂ｬ螳�
		case 0:
			begin_measure(1);
			end_measure(1);
			break;
			//縲撰ｼ代�題�ｪCPU縺ｧ譛�鬮伜━蜈亥ｺｦ縺ｫ
		case 1:
			begin_measure(1);
			act_tsk(TASK1_1);
			break;
			//縲�2縲題�ｪCPU縺ｧ襍ｷ蜍輔＠�ｼ悟ｮ溯｡悟庄閭ｽ迥ｶ諷九↓
		case 2:
			begin_measure(1);
			act_tsk(TASK1_2);
			end_measure(1);
			ter_tsk(TASK1_2);
			break;
			//縲�3縲題�ｪCPU縺ｧ襍ｷ蜍輔☆繧九′�ｼ瑚ｵｷ蠎雁ｾ�縺｡縺ｮ縺溘ａ繧ｭ繝･繝ｼ繧､繝ｳ繧ｰ謨ｰ繧偵う繝ｳ繧ｯ繝ｪ繝｡繝ｳ繝�
		case 3:
			act_tsk(TASK1_3);
			begin_measure(1);
			act_tsk(TASK1_3);
			end_measure(1);
			wup_tsk(TASK1_3);
			wup_tsk(TASK1_3);
			break;
		}
	}

	CPU1_PERF_POST_HOOK;

	syslog(LOG_NOTICE, "==================================");
	syslog(LOG_NOTICE, "(%d)", n);
	syslog(LOG_NOTICE, "----------------------------------");
	print_hist(1);
//	test_finish();
}

/*
 *  PE1 繝｡繧､繝ｳ繧ｿ繧ｹ繧ｯ�ｼ壻ｸｭ蜆ｪ蜈亥ｺｦ
 */
void main_task1(intptr_t exinf)
{
	syslog(LOG_NOTICE, "perf_act_tsk_single for fmp3");
	perf_eval(0);
	perf_eval(1);
	perf_eval(2);
	perf_eval(3);
}

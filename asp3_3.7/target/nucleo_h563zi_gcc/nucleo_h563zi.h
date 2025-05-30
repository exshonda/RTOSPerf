/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2006-2025 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: nucleo_l552zeq.h 1799 2023-04-01 00:50:30Z ertl-komori $
 */

/*
 *		NUCLEO H563ZI サポートモジュール
 */

#ifndef TOPPERS_NUCLEO_H563ZI_H
#define TOPPERS_NUCLEO_H563ZI_H

/*
 *  コアのクロック周波数
 */
#define CPU_CLOCK_HZ	250000000

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 *  ToDo 
 */
#define SIL_DLY_TIM1    63
#define SIL_DLY_TIM2    45

#ifndef TOPPERS_MACRO_ONLY
/*
 *  tecsgen実行時にstm32h5xx_nucleo.hを読み込むとtecsgenがエラーとなるため
 *  tecsgen実行時に必要な定義はこのファイルで行う
 */
#ifndef TECSGEN
#include "stm32h5xx_ll_pwr.h"
#include "stm32h5xx_ll_rcc.h"
#include "stm32h5xx_ll_bus.h"
#include "stm32h5xx_ll_gpio.h"
#else /* !TECSGEN */
#endif /* TECSGEN */
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  USART関連の定義
 */
#define USART_INTNO (USART3_IRQn + 16)
#define USART_NAME  USART3
#define USART_BASE  USART3_BASE

/*
 *  ボーレート
 */
#define BPS_SETTING  (115200)

#ifndef TOPPERS_MACRO_ONLY
#ifndef TECSGEN
/*
 *  USARTのクロックとIOの初期化
 */
Inline void
usart_low_init(void) {
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);

    /* TXピンをハイスピー，プッシュプル，プルアップに設定 */
    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOD, LL_GPIO_PIN_8, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);

    /* RXピンをハイスピー，プッシュプル，プルダウンに設定 */
    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOD, LL_GPIO_PIN_9, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_9, LL_GPIO_PULL_DOWN);

    /* USART3 のクロックを有効に */
    LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
}
#endif /* TECSGEN */

#define NUCLEO_LED1  0x01
#define NUCLEO_LED2  0x02
#define NUCLEO_LED3  0x03

#define LED1_PIN        LL_GPIO_PIN_0
#define LED1_GPIO_PORT  GPIOB

#define LED2_PIN        LL_GPIO_PIN_4
#define LED2_GPIO_PORT  GPIOF

#define LED3_PIN        LL_GPIO_PIN_4
#define LED3_GPIO_PORT  GPIOG

Inline void
nucleo_led_init(void) {
    /* LED1 */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);

    /* LED2 */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
    LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);

    /* LED3 */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
    LL_GPIO_SetPinMode(LED3_GPIO_PORT, LED3_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(LED3_GPIO_PORT, LED3_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(LED3_GPIO_PORT, LED3_PIN, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(LED3_GPIO_PORT, LED3_PIN, LL_GPIO_PULL_NO);
}

Inline void
nucleo_led_on(uint8_t led) {
    if (led == NUCLEO_LED1) {
        LL_GPIO_WriteOutputPort(LED1_GPIO_PORT, 
                                LL_GPIO_ReadOutputPort(LED1_GPIO_PORT) | LED1_PIN);
    }
    else if (led == NUCLEO_LED2) {
        LL_GPIO_WriteOutputPort(LED2_GPIO_PORT, 
                                LL_GPIO_ReadOutputPort(LED2_GPIO_PORT) | LED2_PIN);
    }
    else if (led == NUCLEO_LED3) {
        LL_GPIO_WriteOutputPort(LED3_GPIO_PORT, 
                                LL_GPIO_ReadOutputPort(LED3_GPIO_PORT) | LED3_PIN);
    }
}

Inline void
nucleo_led_off(uint8_t led) {
    if (led == NUCLEO_LED1) {
        LL_GPIO_WriteOutputPort(LED1_GPIO_PORT, 
                                LL_GPIO_ReadOutputPort(LED1_GPIO_PORT) & ~LED1_PIN);
    }
    else if (led == NUCLEO_LED2) {
        LL_GPIO_WriteOutputPort(LED2_GPIO_PORT, 
                                LL_GPIO_ReadOutputPort(LED2_GPIO_PORT) & ~LED2_PIN);
    }
    else if (led == NUCLEO_LED3) {
        LL_GPIO_WriteOutputPort(LED3_GPIO_PORT, 
                                LL_GPIO_ReadOutputPort(LED3_GPIO_PORT) & ~LED3_PIN);
    }
}

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_NUCLEO_H563ZI_H */

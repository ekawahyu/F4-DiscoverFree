/*
 * led.h
 *
 * Created on: Nov 25, 2012
 *     Author: Ekawahyu Susilo
 *
 * Copyright (c) 2012, Chongqing Aisenke Electronic Technology Co., Ltd.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the copyright holder.
 * 
 */

#ifndef LED_H_
#define LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
	LED1,
	LED2,
	LED3,
	LED4,
	LEDn
} led_t;

#define LED1_PIN					GPIO_Pin_6
#define LED1_GPIO					GPIOF
#define LED1_GPIO_CLK				RCC_AHB1Periph_GPIOF

#define LED2_PIN					GPIO_Pin_7
#define LED2_GPIO					GPIOF
#define LED2_GPIO_CLK				RCC_AHB1Periph_GPIOF

#define LED3_PIN					GPIO_Pin_8
#define LED3_GPIO					GPIOF
#define LED3_GPIO_CLK				RCC_AHB1Periph_GPIOF

#define LED4_PIN					GPIO_Pin_9
#define LED4_GPIO					GPIOF
#define LED4_GPIO_CLK				RCC_AHB1Periph_GPIOF

int16_t led_config(led_t led);
int16_t led_deconfig(led_t led);
void led_off(led_t led);
void led_on(led_t led);
void led_toggle(led_t led);

#ifdef __cplusplus
}
#endif

#endif /* LED_H_ */

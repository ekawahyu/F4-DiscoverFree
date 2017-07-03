/*
 * led.c
 *
 * Created on: Jul 20, 2012
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

#include <stdlib.h>
#include <string.h>
#include "led.h"

uint16_t led_pin[LEDn]       = {
		LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN
};

GPIO_TypeDef* led_gpio[LEDn] = {
		LED1_GPIO, LED2_GPIO, LED3_GPIO, LED4_GPIO
};

uint32_t led_gpio_clk[LEDn]  = {
		LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK
};

int16_t led_config(led_t led)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Capture wrong led number */
	if (led >= LEDn) return -1;

	/* Enable GPIO peripheral clock */
	RCC_AHB1PeriphClockCmd(led_gpio_clk[led], ENABLE);

	/* Configure LED pin as output push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = led_pin[led];
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(led_gpio[led], &GPIO_InitStructure);

	return 0;
}

int16_t led_deconfig(led_t led)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure unused GPIO port pins in Analog Input mode (floating input
	 * trigger OFF), this will reduce the power consumption and increase the
	 * device immunity against EMI/EMC */

	/* Capture wrong led number */
	if (led >= LEDn) return -1;

	/* Configure LED pin as analog input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = led_pin[led];
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(led_gpio[led], &GPIO_InitStructure);

	return 0;
}

void led_off(led_t led)
{
	if (led >= LEDn) return;

	GPIO_ResetBits(led_gpio[led], led_pin[led]);
}

void led_on(led_t led)
{
	if (led >= LEDn) return;

	GPIO_SetBits(led_gpio[led], led_pin[led]);
}

void led_toggle(led_t led)
{
	if (led >= LEDn) return;

	if (GPIO_ReadOutputDataBit(led_gpio[led], led_pin[led]) == Bit_RESET)
		GPIO_SetBits(led_gpio[led], led_pin[led]);
	else
		GPIO_ResetBits(led_gpio[led], led_pin[led]);
}

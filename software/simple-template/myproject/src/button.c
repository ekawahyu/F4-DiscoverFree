/*
 * button.c
 *
 * Created on: Nov 26, 2012
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

#include "button.h"

uint16_t button_pin[BUTTONn]       = {
		BUTTON_S1_PIN, BUTTON_S2_PIN
};

GPIO_TypeDef* button_gpio[BUTTONn] = {
		BUTTON_S1_GPIO, BUTTON_S2_GPIO
};

uint32_t button_gpio_clk[BUTTONn]  = {
		BUTTON_S1_GPIO_CLK, BUTTON_S2_GPIO_CLK
};

int16_t button_config(button_t button)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Capture wrong button number */
	if (button >= BUTTONn || button == BUTTON_S1) return -1;

	/* Enable GPIO peripheral clock */
	RCC_AHB1PeriphClockCmd(button_gpio_clk[button], ENABLE);

	/* Configure button pin as digital input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = button_pin[button];
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(button_gpio[button], &GPIO_InitStructure);

	return 0;
}

int16_t button_deconfig(button_t button)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure unused GPIO port pins in Analog Input mode (floating input
	 * trigger OFF), this will reduce the power consumption and increase the
	 * device immunity against EMI/EMC */

	/* Capture wrong button number */
	if (button >= BUTTONn || button == BUTTON_S1) return -1;

	/* Configure button pin as analog input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = button_pin[button];
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(button_gpio[button], &GPIO_InitStructure);

	return 0;
}

int16_t button_getstate(button_t button)
{
	/* Capture wrong button number */
	if (button >= BUTTONn || button == BUTTON_S1) return -1;

	return GPIO_ReadInputDataBit(button_gpio[button], button_pin[button]);
}


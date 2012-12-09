/*
 * servo.c
 *
 * Created on: Dec 8, 2012
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

#include "servo.h"

const volatile uint16_t period = 2000;
volatile uint16_t period_count = 0;

volatile servo_object_t servo_motor[SERVOn] = { {0} };

uint16_t servo_pin[SERVOn]       = {
		SERVO1_PIN,  SERVO2_PIN,  SERVO3_PIN,  SERVO4_PIN,
		SERVO5_PIN,  SERVO6_PIN,  SERVO7_PIN,  SERVO8_PIN,
		SERVO9_PIN,  SERVO10_PIN, SERVO11_PIN, SERVO12_PIN,
		SERVO13_PIN, SERVO14_PIN, SERVO15_PIN, SERVO16_PIN,
		SERVO17_PIN, SERVO18_PIN, SERVO19_PIN, SERVO20_PIN,
		SERVO21_PIN, SERVO22_PIN, SERVO23_PIN, SERVO24_PIN,
		SERVO25_PIN, SERVO26_PIN
};

GPIO_TypeDef* servo_gpio[SERVOn] = {
		SERVO1_GPIO,  SERVO2_GPIO,  SERVO3_GPIO,  SERVO4_GPIO,
		SERVO5_GPIO,  SERVO6_GPIO,  SERVO7_GPIO,  SERVO8_GPIO,
		SERVO9_GPIO,  SERVO10_GPIO, SERVO11_GPIO, SERVO12_GPIO,
		SERVO13_GPIO, SERVO14_GPIO, SERVO15_GPIO, SERVO16_GPIO,
		SERVO17_GPIO, SERVO18_GPIO, SERVO19_GPIO, SERVO20_GPIO,
		SERVO21_GPIO, SERVO22_GPIO, SERVO23_GPIO, SERVO24_GPIO,
		SERVO25_GPIO, SERVO26_GPIO
};

uint32_t servo_gpio_clk[SERVOn]  = {
		SERVO1_GPIO_CLK,  SERVO2_GPIO_CLK,  SERVO3_GPIO_CLK,  SERVO4_GPIO_CLK,
		SERVO5_GPIO_CLK,  SERVO6_GPIO_CLK,  SERVO7_GPIO_CLK,  SERVO8_GPIO_CLK,
		SERVO9_GPIO_CLK,  SERVO10_GPIO_CLK, SERVO11_GPIO_CLK, SERVO12_GPIO_CLK,
		SERVO13_GPIO_CLK, SERVO14_GPIO_CLK, SERVO15_GPIO_CLK, SERVO16_GPIO_CLK,
		SERVO17_GPIO_CLK, SERVO18_GPIO_CLK, SERVO19_GPIO_CLK, SERVO4_GPIO_CLK,
		SERVO21_GPIO_CLK, SERVO22_GPIO_CLK, SERVO23_GPIO_CLK, SERVO24_GPIO_CLK,
		SERVO25_GPIO_CLK, SERVO26_GPIO_CLK
};

int16_t servo_config(servo_t servo)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Capture wrong servo number */
	if (servo >= SERVOn) return -1;

	/* Enable GPIO peripheral clock */
	RCC_AHB1PeriphClockCmd(servo_gpio_clk[servo], ENABLE);

	/* Configure GPIO pin as output push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = servo_pin[servo];
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(servo_gpio[servo], &GPIO_InitStructure);

	return 0;
}

int16_t servo_deconfig(servo_t servo)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure unused GPIO port pins in Analog Input mode (floating input
	 * trigger OFF), this will reduce the power consumption and increase the
	 * device immunity against EMI/EMC */

	/* Capture wrong led number */
	if (servo >= SERVOn) return -1;

	/* Configure LED pin as analog input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = servo_pin[servo];
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(servo_gpio[servo], &GPIO_InitStructure);

	return 0;
}

void servo_reset(servo_t servo)
{
	if (servo >= SERVOn) return;

	GPIO_ResetBits(servo_gpio[servo], servo_pin[servo]);
}

void servo_set(servo_t servo)
{
	if (servo >= SERVOn) return;

	GPIO_SetBits(servo_gpio[servo], servo_pin[servo]);
}

void servo_controller_periodic_handler(void)
{
	int i;

	period_count++;

	for (i = 0; i < SERVOn; i++)
		if (period_count >= servo_motor[i].current_position) servo_reset(i);

	if (period_count >= period) {
		for (i = 0; i < SERVOn; i++) servo_set(i);
		period_count = 0;
	}
}

void servo_power_off(servo_t servo)
{
	if (servo >= SERVOn) return;

	servo_deconfig(servo);
}

void servo_power_on(servo_t servo)
{
	if (servo >= SERVOn) return;

	servo_config(servo);
}

void servo_move(servo_t servo, uint16_t position)
{
	if (servo >= SERVOn) return;

	servo_motor[servo].current_position = position;
}

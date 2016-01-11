/*
 * servo.h
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

#ifndef SERVO_H_
#define SERVO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
	SERVO1,
	SERVO2,
	SERVO3,
	SERVO4,
	SERVO5,
	SERVO6,
	SERVO7,
	SERVO8,
	SERVO9,
	SERVO10,
	SERVO11,
	SERVO12,
	SERVO13,
	SERVO14,
	SERVO15,
	SERVO16,
	SERVO17,
	SERVO18,
	SERVO19,
	SERVO20,
	SERVO21,
	SERVO22,
	SERVO23,
	SERVO24,
	SERVO25,
	SERVO26,
	SERVOn
} servo_t;

#define SERVO1_PIN					GPIO_Pin_2
#define SERVO1_GPIO					GPIOH
#define SERVO1_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO2_PIN					GPIO_Pin_3
#define SERVO2_GPIO					GPIOH
#define SERVO2_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO3_PIN					GPIO_Pin_4
#define SERVO3_GPIO					GPIOH
#define SERVO3_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO4_PIN					GPIO_Pin_5
#define SERVO4_GPIO					GPIOH
#define SERVO4_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO5_PIN					GPIO_Pin_6
#define SERVO5_GPIO					GPIOH
#define SERVO5_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO6_PIN					GPIO_Pin_7
#define SERVO6_GPIO					GPIOH
#define SERVO6_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO7_PIN					GPIO_Pin_8
#define SERVO7_GPIO					GPIOH
#define SERVO7_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO8_PIN					GPIO_Pin_9
#define SERVO8_GPIO					GPIOH
#define SERVO8_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO9_PIN					GPIO_Pin_10
#define SERVO9_GPIO					GPIOH
#define SERVO9_GPIO_CLK				RCC_AHB1Periph_GPIOH

#define SERVO10_PIN					GPIO_Pin_11
#define SERVO10_GPIO				GPIOH
#define SERVO10_GPIO_CLK			RCC_AHB1Periph_GPIOH

#define SERVO11_PIN					GPIO_Pin_12
#define SERVO11_GPIO				GPIOH
#define SERVO11_GPIO_CLK			RCC_AHB1Periph_GPIOH

#define SERVO12_PIN					GPIO_Pin_13
#define SERVO12_GPIO				GPIOH
#define SERVO12_GPIO_CLK			RCC_AHB1Periph_GPIOH

#define SERVO13_PIN					GPIO_Pin_14
#define SERVO13_GPIO				GPIOH
#define SERVO13_GPIO_CLK			RCC_AHB1Periph_GPIOH

#define SERVO14_PIN					GPIO_Pin_15
#define SERVO14_GPIO				GPIOH
#define SERVO14_GPIO_CLK			RCC_AHB1Periph_GPIOH

#define SERVO15_PIN					GPIO_Pin_0
#define SERVO15_GPIO				GPIOI
#define SERVO15_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO16_PIN					GPIO_Pin_1
#define SERVO16_GPIO				GPIOI
#define SERVO16_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO17_PIN					GPIO_Pin_2
#define SERVO17_GPIO				GPIOI
#define SERVO17_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO18_PIN					GPIO_Pin_3
#define SERVO18_GPIO				GPIOI
#define SERVO18_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO19_PIN					GPIO_Pin_4
#define SERVO19_GPIO				GPIOI
#define SERVO19_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO20_PIN					GPIO_Pin_5
#define SERVO20_GPIO				GPIOI
#define SERVO20_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO21_PIN					GPIO_Pin_6
#define SERVO21_GPIO				GPIOI
#define SERVO21_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO22_PIN					GPIO_Pin_7
#define SERVO22_GPIO				GPIOI
#define SERVO22_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO23_PIN					GPIO_Pin_8
#define SERVO23_GPIO				GPIOI
#define SERVO23_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO24_PIN					GPIO_Pin_9
#define SERVO24_GPIO				GPIOI
#define SERVO24_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO25_PIN					GPIO_Pin_10
#define SERVO25_GPIO				GPIOI
#define SERVO25_GPIO_CLK			RCC_AHB1Periph_GPIOI

#define SERVO26_PIN					GPIO_Pin_11
#define SERVO26_GPIO				GPIOI
#define SERVO26_GPIO_CLK			RCC_AHB1Periph_GPIOI

typedef struct {
	uint16_t position;
} servo_object_t;

void servo_controller_periodic_handler(void);

int16_t servo_config(servo_t servo);
int16_t servo_deconfig(servo_t servo);
void servo_power_off(servo_t servo);
void servo_power_on(servo_t servo);
void servo_move(servo_t servo, uint16_t position);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_H_ */

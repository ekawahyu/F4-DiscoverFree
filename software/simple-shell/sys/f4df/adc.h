/*
 * adc.h
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

#ifndef ADC_H_
#define ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
	ANALOG1,
	ANALOG2,
	ANALOG3,
	ANALOG4,
	ANALOG5,
	ANALOG6,
	ANALOG7,
	ANALOG8,
	ANALOG9,
	ANALOG10,
	ANALOG11,
	ANALOG12,
	ANALOG13,
	ANALOG14,
	ANALOG15,
	ANALOG16,
	ANALOG17,
	ANALOG18,
	ANALOG19,
	ANALOG20,
	ANALOG21,
	ANALOG22,
	ANALOG23,
	ANALOG24,
	ANALOGn
} analog_t;

#define ANALOG1_PIN					GPIO_Pin_0				/* ADC123_IN0 */
#define ANALOG1_GPIO				GPIOA
#define ANALOG1_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG1_CHANNEL				ADC_Channel_0

#define ANALOG2_PIN					GPIO_Pin_1				/* ADC123_IN1 */
#define ANALOG2_GPIO				GPIOA
#define ANALOG2_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG2_CHANNEL				ADC_Channel_1

#define ANALOG3_PIN					GPIO_Pin_2				/* ADC123_IN2 */
#define ANALOG3_GPIO				GPIOA
#define ANALOG3_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG3_CHANNEL				ADC_Channel_2

#define ANALOG4_PIN					GPIO_Pin_3				/* ADC123_IN3 */
#define ANALOG4_GPIO				GPIOA
#define ANALOG4_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG4_CHANNEL				ADC_Channel_3

#define ANALOG5_PIN					GPIO_Pin_4				/* ADC12_IN4 */
#define ANALOG5_GPIO				GPIOA
#define ANALOG5_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG5_CHANNEL				ADC_Channel_4

#define ANALOG6_PIN					GPIO_Pin_5				/* ADC12_IN5 */
#define ANALOG6_GPIO				GPIOA
#define ANALOG6_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG6_CHANNEL				ADC_Channel_5

#define ANALOG7_PIN					GPIO_Pin_6				/* ADC12_IN6 */
#define ANALOG7_GPIO				GPIOA
#define ANALOG7_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG7_CHANNEL				ADC_Channel_6

#define ANALOG8_PIN					GPIO_Pin_7				/* ADC12_IN7 */
#define ANALOG8_GPIO				GPIOA
#define ANALOG8_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define ANALOG8_CHANNEL				ADC_Channel_7

#define ANALOG9_PIN					GPIO_Pin_0				/* ADC12_IN8 */
#define ANALOG9_GPIO				GPIOB
#define ANALOG9_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define ANALOG9_CHANNEL				ADC_Channel_8

#define ANALOG10_PIN				GPIO_Pin_1				/* ADC12_IN9 */
#define ANALOG10_GPIO				GPIOB
#define ANALOG10_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define ANALOG10_CHANNEL			ADC_Channel_9

#define ANALOG11_PIN				GPIO_Pin_0				/* ADC123_IN10 */
#define ANALOG11_GPIO				GPIOC
#define ANALOG11_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define ANALOG11_CHANNEL			ADC_Channel_10

#define ANALOG12_PIN				GPIO_Pin_1				/* ADC123_IN11 */
#define ANALOG12_GPIO				GPIOC
#define ANALOG12_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define ANALOG12_CHANNEL			ADC_Channel_11

#define ANALOG13_PIN				GPIO_Pin_2				/* ADC123_IN12 */
#define ANALOG13_GPIO				GPIOC
#define ANALOG13_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define ANALOG13_CHANNEL			ADC_Channel_12

#define ANALOG14_PIN				GPIO_Pin_3				/* ADC123_IN13 */
#define ANALOG14_GPIO				GPIOC
#define ANALOG14_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define ANALOG14_CHANNEL			ADC_Channel_13

#define ANALOG15_PIN				GPIO_Pin_4				/* ADC12_IN14 */
#define ANALOG15_GPIO				GPIOC
#define ANALOG15_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define ANALOG15_CHANNEL			ADC_Channel_14

#define ANALOG16_PIN				GPIO_Pin_5				/* ADC12_IN15 */
#define ANALOG16_GPIO				GPIOC
#define ANALOG16_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define ANALOG16_CHANNEL			ADC_Channel_15

#define ANALOG17_PIN				GPIO_Pin_3				/* ADC3_IN9 */
#define ANALOG17_GPIO				GPIOF
#define ANALOG17_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG17_CHANNEL			ADC_Channel_9

#define ANALOG18_PIN				GPIO_Pin_4				/* ADC3_IN14 */
#define ANALOG18_GPIO				GPIOF
#define ANALOG18_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG18_CHANNEL			ADC_Channel_14

#define ANALOG19_PIN				GPIO_Pin_5				/* ADC3_IN15 */
#define ANALOG19_GPIO				GPIOF
#define ANALOG19_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG19_CHANNEL			ADC_Channel_15

#define ANALOG20_PIN				GPIO_Pin_6				/* ADC3_IN4 */
#define ANALOG20_GPIO				GPIOF
#define ANALOG20_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG20_CHANNEL			ADC_Channel_4

#define ANALOG21_PIN				GPIO_Pin_7				/* ADC3_IN5 */
#define ANALOG21_GPIO				GPIOF
#define ANALOG21_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG21_CHANNEL			ADC_Channel_5

#define ANALOG22_PIN				GPIO_Pin_8				/* ADC3_IN6 */
#define ANALOG22_GPIO				GPIOF
#define ANALOG22_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG22_CHANNEL			ADC_Channel_6

#define ANALOG23_PIN				GPIO_Pin_9				/* ADC3_IN7 */
#define ANALOG23_GPIO				GPIOF
#define ANALOG23_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG23_CHANNEL			ADC_Channel_7

#define ANALOG24_PIN				GPIO_Pin_10				/* ADC3_IN8 */
#define ANALOG24_GPIO				GPIOF
#define ANALOG24_GPIO_CLK			RCC_AHB1Periph_GPIOF
#define ANALOG24_CHANNEL			ADC_Channel_8

int16_t analog_config(analog_t analog);
int16_t analog_deconfig(analog_t analog);
int16_t analog_read(analog_t analog);

#ifdef __cplusplus
}
#endif

#endif /* ADC_H_ */

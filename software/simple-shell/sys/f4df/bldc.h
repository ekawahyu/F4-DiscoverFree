/*
 * bldc.h
 *
 * Created on: Dec 13, 2012
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

#ifndef BLDC_H_
#define BLDC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum
{
	BLDC1,
	BLDC2,
	BLDC3,
	BLDC4,
	BLDCn
} bldc_t;

typedef enum
{
	COMMUTATION0, /* idle state */
	COMMUTATION1,
	COMMUTATION2,
	COMMUTATION3,
	COMMUTATION4,
	COMMUTATION5,
	COMMUTATION6,
	COMMUTATIONn
} commutation_t;

typedef enum
{
	BLDC_IDLE,
	BLDC_READY,
	BLDC_IGNITION,
	BLDC_STARTUP,
	BLDC_TRANSITION,
	BLDC_BEMF,
	BLDC_SLOWING_DOWN
} bldc_state_t;

extern int f_CW1, f_CCW1, f_CW2, f_CCW2;
extern int f_CW3, f_CCW3, f_CW4, f_CCW4;
extern int32_t BRSLS1_POS, BRSLS2_POS;
extern int32_t BRSLS3_POS, BRSLS4_POS;

#define BLDC1_U1_PIN					GPIO_Pin_4
#define BLDC1_U1_GPIO					GPIOD
#define BLDC1_U1_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC1_U2_PIN					GPIO_Pin_5
#define BLDC1_U2_GPIO					GPIOD
#define BLDC1_U2_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC1_V1_PIN					GPIO_Pin_6
#define BLDC1_V1_GPIO					GPIOD
#define BLDC1_V1_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC1_V2_PIN					GPIO_Pin_7
#define BLDC1_V2_GPIO					GPIOD
#define BLDC1_V2_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC1_W1_PIN					GPIO_Pin_8
#define BLDC1_W1_GPIO					GPIOD
#define BLDC1_W1_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC1_W2_PIN					GPIO_Pin_9
#define BLDC1_W2_GPIO					GPIOD
#define BLDC1_W2_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC1_U_BEMF_PIN				ANALOG2
#define BLDC1_V_BEMF_PIN				ANALOG3
#define BLDC1_W_BEMF_PIN				ANALOG5


#define BLDC2_U1_PIN					GPIO_Pin_10
#define BLDC2_U1_GPIO					GPIOD
#define BLDC2_U1_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC2_U2_PIN					GPIO_Pin_11
#define BLDC2_U2_GPIO					GPIOD
#define BLDC2_U2_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC2_V1_PIN					GPIO_Pin_12
#define BLDC2_V1_GPIO					GPIOD
#define BLDC2_V1_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC2_V2_PIN					GPIO_Pin_13
#define BLDC2_V2_GPIO					GPIOD
#define BLDC2_V2_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC2_W1_PIN					GPIO_Pin_14
#define BLDC2_W1_GPIO					GPIOD
#define BLDC2_W1_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC2_W2_PIN					GPIO_Pin_15
#define BLDC2_W2_GPIO					GPIOD
#define BLDC2_W2_GPIO_CLK				RCC_AHB1Periph_GPIOD

#define BLDC2_U_BEMF_PIN				ANALOG6
#define BLDC2_V_BEMF_PIN				ANALOG7
#define BLDC2_W_BEMF_PIN				ANALOG8


#define BLDC3_U1_PIN					GPIO_Pin_4
#define BLDC3_U1_GPIO					GPIOE
#define BLDC3_U1_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC3_U2_PIN					GPIO_Pin_5
#define BLDC3_U2_GPIO					GPIOE
#define BLDC3_U2_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC3_V1_PIN					GPIO_Pin_6
#define BLDC3_V1_GPIO					GPIOE
#define BLDC3_V1_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC3_V2_PIN					GPIO_Pin_7
#define BLDC3_V2_GPIO					GPIOE
#define BLDC3_V2_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC3_W1_PIN					GPIO_Pin_8
#define BLDC3_W1_GPIO					GPIOE
#define BLDC3_W1_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC3_W2_PIN					GPIO_Pin_9
#define BLDC3_W2_GPIO					GPIOE
#define BLDC3_W2_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC3_U_BEMF_PIN				ANALOG12
#define BLDC3_V_BEMF_PIN				ANALOG15
#define BLDC3_W_BEMF_PIN				ANALOG16


#define BLDC4_U1_PIN					GPIO_Pin_10
#define BLDC4_U1_GPIO					GPIOE
#define BLDC4_U1_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC4_U2_PIN					GPIO_Pin_11
#define BLDC4_U2_GPIO					GPIOE
#define BLDC4_U2_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC4_V1_PIN					GPIO_Pin_12
#define BLDC4_V1_GPIO					GPIOE
#define BLDC4_V1_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC4_V2_PIN					GPIO_Pin_13
#define BLDC4_V2_GPIO					GPIOE
#define BLDC4_V2_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC4_W1_PIN					GPIO_Pin_14
#define BLDC4_W1_GPIO					GPIOE
#define BLDC4_W1_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC4_W2_PIN					GPIO_Pin_15
#define BLDC4_W2_GPIO					GPIOE
#define BLDC4_W2_GPIO_CLK				RCC_AHB1Periph_GPIOE

#define BLDC4_U_BEMF_PIN				ANALOG17
#define BLDC4_V_BEMF_PIN				ANALOG18
#define BLDC4_W_BEMF_PIN				ANALOG19

#define BLDC1_CW_BUTTON					BUTTON_S10
#define BLDC1_CCW_BUTTON				BUTTON_S9
#define BLDC2_CW_BUTTON					BUTTON_S8
#define BLDC2_CCW_BUTTON				BUTTON_S7
#define BLDC3_CW_BUTTON					BUTTON_S6
#define BLDC3_CCW_BUTTON				BUTTON_S5
#define BLDC4_CW_BUTTON					BUTTON_S4
#define BLDC4_CCW_BUTTON				BUTTON_S3

void bldc_controller_periodic_handler(void);

int16_t bldc_config(bldc_t brushless);
int16_t bldc_deconfig(bldc_t brushless);
void bldc_cw(bldc_t brushless);
void bldc_ccw(bldc_t brushless);
void bldc_stop(bldc_t brushless);
void bldc_step_cw(bldc_t brushless, int32_t step);
void bldc_step_ccw(bldc_t brushless, int32_t step);
int32_t bldc_get_position(bldc_t brushless);
void bldc_reset_position(bldc_t brushless);
int32_t bldc_get_anchor(bldc_t brushless);
void bldc_set_anchor(bldc_t brushless, int32_t position);
void bldc_button_mode_on(bldc_t brushless);
void bldc_button_mode_off(bldc_t brushless);
uint16_t bldc_get_button_mode(bldc_t brushless);
void bldc_bemf_mode_on(bldc_t brushless);
void bldc_bemf_mode_off(bldc_t brushless);
uint16_t bldc_get_bemf_mode(bldc_t brushless);
int16_t bldc_get_direction(bldc_t brushless);

#ifdef __cplusplus
}
#endif

#endif /* BLDC_H_ */

/*
 * bldc.c
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

#include "compiler.h"
#include "adc.h"
#include "button.h"
#include "bldc.h"

void bldc_commutation(bldc_t brushless, commutation_t commutation);

volatile uint16_t bldc_tick = 200; /* us */
volatile uint16_t bldc_startup_tick = 1000; /* us */

volatile uint16_t bldc_period = 0;
volatile uint16_t bldc_period_count = 0;
volatile uint16_t zero_crossing = 1800;

typedef struct
{
	bldc_state_t state;
	uint16_t commutation;
	uint16_t startup_count;
	uint16_t startup_period; /* relative to bldc_tick */
	uint16_t startup_steps;
	uint16_t startup_steps_count;
	uint16_t bemf_mode;
	int32_t  position;
	int32_t  anchor;
	int16_t  direction;
	int16_t  button_mode;
} bldc_object_t;

bldc_object_t brushless_motor[BLDCn] = {
		[0 ... (BLDCn-1)] = {
		.state          = BLDC_IDLE,
		.commutation    = COMMUTATION1,
		.startup_count  = 0,
		.startup_period = 0,
		.startup_steps  = 60,
		.startup_count  = 0,
		.bemf_mode      = 1,
		.position       = 0,
		.anchor         = 0,
		.direction      = 0,
		.button_mode    = 1,
} };

uint16_t bldc_u1_pin[BLDCn]       = {
		BLDC1_U1_PIN,  BLDC2_U1_PIN,
		BLDC3_U1_PIN,  BLDC4_U1_PIN
};

uint16_t bldc_u2_pin[BLDCn]       = {
		BLDC1_U2_PIN,  BLDC2_U2_PIN,
		BLDC3_U2_PIN,  BLDC4_U2_PIN
};

uint16_t bldc_v1_pin[BLDCn]       = {
		BLDC1_V1_PIN,  BLDC2_V1_PIN,
		BLDC3_V1_PIN,  BLDC4_V1_PIN
};

uint16_t bldc_v2_pin[BLDCn]       = {
		BLDC1_V2_PIN,  BLDC2_V2_PIN,
		BLDC3_V2_PIN,  BLDC4_V2_PIN
};

uint16_t bldc_w1_pin[BLDCn]       = {
		BLDC1_W1_PIN,  BLDC2_W1_PIN,
		BLDC3_W1_PIN,  BLDC4_W1_PIN
};

uint16_t bldc_w2_pin[BLDCn]       = {
		BLDC1_W2_PIN,  BLDC2_W2_PIN,
		BLDC3_W2_PIN,  BLDC4_W2_PIN
};

analog_t bldc_u_bemf_pin[BLDCn]   = {
		BLDC1_U_BEMF_PIN, BLDC2_U_BEMF_PIN,
		BLDC3_U_BEMF_PIN, BLDC4_U_BEMF_PIN
};

analog_t bldc_v_bemf_pin[BLDCn]   = {
		BLDC1_V_BEMF_PIN, BLDC2_V_BEMF_PIN,
		BLDC3_V_BEMF_PIN, BLDC4_V_BEMF_PIN
};

analog_t bldc_w_bemf_pin[BLDCn]   = {
		BLDC1_W_BEMF_PIN, BLDC2_W_BEMF_PIN,
		BLDC3_W_BEMF_PIN, BLDC4_W_BEMF_PIN
};

button_t bldc_button_cw_pin[BLDCn] = {
		BLDC1_CW_BUTTON, BLDC2_CW_BUTTON,
		BLDC3_CW_BUTTON, BLDC4_CW_BUTTON
};

button_t bldc_button_ccw_pin[BLDCn] = {
		BLDC1_CCW_BUTTON, BLDC2_CCW_BUTTON,
		BLDC3_CCW_BUTTON, BLDC4_CCW_BUTTON
};

GPIO_TypeDef* bldc_u1_gpio[BLDCn] = {
		BLDC1_U1_GPIO,  BLDC2_U1_GPIO,
		BLDC3_U1_GPIO,  BLDC4_U1_GPIO
};

GPIO_TypeDef* bldc_u2_gpio[BLDCn] = {
		BLDC1_U2_GPIO,  BLDC2_U2_GPIO,
		BLDC3_U2_GPIO,  BLDC4_U2_GPIO
};

GPIO_TypeDef* bldc_v1_gpio[BLDCn] = {
		BLDC1_V1_GPIO,  BLDC2_V1_GPIO,
		BLDC3_V1_GPIO,  BLDC4_V1_GPIO
};

GPIO_TypeDef* bldc_v2_gpio[BLDCn] = {
		BLDC1_V2_GPIO,  BLDC2_V2_GPIO,
		BLDC3_V2_GPIO,  BLDC4_V2_GPIO
};

GPIO_TypeDef* bldc_w1_gpio[BLDCn] = {
		BLDC1_W1_GPIO,  BLDC2_W1_GPIO,
		BLDC3_W1_GPIO,  BLDC4_W1_GPIO
};

GPIO_TypeDef* bldc_w2_gpio[BLDCn] = {
		BLDC1_W2_GPIO,  BLDC2_W2_GPIO,
		BLDC3_W2_GPIO,  BLDC4_W2_GPIO
};

uint32_t bldc_u1_gpio_clk[BLDCn]  = {
		BLDC1_U1_GPIO_CLK,  BLDC2_U1_GPIO_CLK,
		BLDC3_U1_GPIO_CLK,  BLDC4_U1_GPIO_CLK
};

uint32_t bldc_u2_gpio_clk[BLDCn]  = {
		BLDC1_U2_GPIO_CLK,  BLDC2_U2_GPIO_CLK,
		BLDC3_U2_GPIO_CLK,  BLDC4_U2_GPIO_CLK
};

uint32_t bldc_v1_gpio_clk[BLDCn]  = {
		BLDC1_V1_GPIO_CLK,  BLDC2_V1_GPIO_CLK,
		BLDC3_V1_GPIO_CLK,  BLDC4_V1_GPIO_CLK
};

uint32_t bldc_v2_gpio_clk[BLDCn]  = {
		BLDC1_V2_GPIO_CLK,  BLDC2_V2_GPIO_CLK,
		BLDC3_V2_GPIO_CLK,  BLDC4_V2_GPIO_CLK
};

uint32_t bldc_w1_gpio_clk[BLDCn]  = {
		BLDC1_W1_GPIO_CLK,  BLDC2_W1_GPIO_CLK,
		BLDC3_W1_GPIO_CLK,  BLDC4_W1_GPIO_CLK
};

uint32_t bldc_w2_gpio_clk[BLDCn]  = {
		BLDC1_W2_GPIO_CLK,  BLDC2_W2_GPIO_CLK,
		BLDC3_W2_GPIO_CLK,  BLDC4_W2_GPIO_CLK
};

int16_t bldc_peripheral_config(bldc_t brushless)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return -1;

	/* Enable GPIO peripheral clock */
	RCC_AHB1PeriphClockCmd(bldc_u1_gpio_clk[brushless], ENABLE);
	RCC_AHB1PeriphClockCmd(bldc_u2_gpio_clk[brushless], ENABLE);
	RCC_AHB1PeriphClockCmd(bldc_v1_gpio_clk[brushless], ENABLE);
	RCC_AHB1PeriphClockCmd(bldc_v2_gpio_clk[brushless], ENABLE);
	RCC_AHB1PeriphClockCmd(bldc_w1_gpio_clk[brushless], ENABLE);
	RCC_AHB1PeriphClockCmd(bldc_w2_gpio_clk[brushless], ENABLE);

	/* Configure GPIO pin as output push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = bldc_u1_pin[brushless];
	GPIO_Init(bldc_u1_gpio[brushless], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = bldc_u2_pin[brushless];
	GPIO_Init(bldc_u2_gpio[brushless], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = bldc_v1_pin[brushless];
	GPIO_Init(bldc_v1_gpio[brushless], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = bldc_v2_pin[brushless];
	GPIO_Init(bldc_v2_gpio[brushless], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = bldc_w1_pin[brushless];
	GPIO_Init(bldc_w1_gpio[brushless], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = bldc_w2_pin[brushless];
	GPIO_Init(bldc_w2_gpio[brushless], &GPIO_InitStructure);

	return 0;
}

void bldc_u1_on(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_SetBits(bldc_u1_gpio[brushless], bldc_u1_pin[brushless]);
}

void bldc_u1_off(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_ResetBits(bldc_u1_gpio[brushless], bldc_u1_pin[brushless]);
}

void bldc_u2_on(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_SetBits(bldc_u2_gpio[brushless], bldc_u2_pin[brushless]);
}

void bldc_u2_off(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_ResetBits(bldc_u2_gpio[brushless], bldc_u2_pin[brushless]);
}

void bldc_v1_on(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_SetBits(bldc_v1_gpio[brushless], bldc_v1_pin[brushless]);
}

void bldc_v1_off(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_ResetBits(bldc_v1_gpio[brushless], bldc_v1_pin[brushless]);
}

void bldc_v2_on(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_SetBits(bldc_v2_gpio[brushless], bldc_v2_pin[brushless]);
}

void bldc_v2_off(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_ResetBits(bldc_v2_gpio[brushless], bldc_v2_pin[brushless]);
}

void bldc_w1_on(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_SetBits(bldc_w1_gpio[brushless], bldc_w1_pin[brushless]);
}

void bldc_w1_off(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_ResetBits(bldc_w1_gpio[brushless], bldc_w1_pin[brushless]);
}

void bldc_w2_on(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_SetBits(bldc_w2_gpio[brushless], bldc_w2_pin[brushless]);
}

void bldc_w2_off(bldc_t brushless)
{
	if (brushless >= BLDCn) return;

	GPIO_ResetBits(bldc_w2_gpio[brushless], bldc_w2_pin[brushless]);
}

uint16_t bldc_u_read_bemf(bldc_t brushless)
{
	return 0;
}

uint16_t bldc_v_read_bemf(bldc_t brushless)
{
	return 0;
}

uint16_t bldc_w_read_bemf(bldc_t brushless)
{
	return 0;
}

int16_t bldc_config(bldc_t brushless)
{
	bldc_period = (bldc_tick/(1000000/CLOCKS_PER_SEC));
	brushless_motor[brushless].startup_period = (bldc_startup_tick/bldc_tick);

	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return -1;

	bldc_peripheral_config(brushless);
	analog_config(bldc_u_bemf_pin[brushless]);
	analog_config(bldc_v_bemf_pin[brushless]);
	analog_config(bldc_w_bemf_pin[brushless]);
	button_config_with_mode(bldc_button_cw_pin[brushless], BUTTON_MODE_PULLUP);
	button_config_with_mode(bldc_button_ccw_pin[brushless], BUTTON_MODE_PULLUP);

	bldc_commutation(brushless, COMMUTATION0);

	return 0;
}

int16_t bldc_deconfig(bldc_t brushless)
{
	/* not implemented */

	return -1;
}

void bldc_cw(bldc_t brushless)
{
	brushless_motor[brushless].direction = 1;
}

void bldc_ccw(bldc_t brushless)
{
	brushless_motor[brushless].direction = -1;
}

void bldc_stop(bldc_t brushless)
{
	brushless_motor[brushless].direction = 0;
}

void bldc_step_cw(bldc_t brushless, int32_t step)
{
	int position = 0;

	position = bldc_get_position(brushless);
	bldc_set_anchor(brushless, position + step);
	if (step > 0) bldc_cw(brushless);
	if (step < 0) bldc_ccw(brushless);
}

void bldc_step_ccw(bldc_t brushless, int32_t step)
{
	int position = 0;

	position = bldc_get_position(brushless);
	bldc_set_anchor(brushless, position - step);
	if (step > 0) bldc_ccw(brushless);
	if (step < 0) bldc_cw(brushless);
}

int32_t bldc_get_position(bldc_t brushless)
{
	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return -1;

	return brushless_motor[brushless].position;
}

void bldc_reset_position(bldc_t brushless)
{
	brushless_motor[brushless].position = 0;
}

int32_t bldc_get_anchor(bldc_t brushless)
{
	return brushless_motor[brushless].anchor;
}

void bldc_set_anchor(bldc_t brushless, int32_t position)
{
	brushless_motor[brushless].anchor = position;
}

void bldc_button_mode_on(bldc_t brushless)
{
	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return;

	brushless_motor[brushless].button_mode = 1;
}

void bldc_button_mode_off(bldc_t brushless)
{
	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return;

	brushless_motor[brushless].button_mode = 0;
}

uint16_t bldc_get_button_mode(bldc_t brushless)
{
	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return -1;

	return brushless_motor[brushless].button_mode;
}

void bldc_bemf_mode_on(bldc_t brushless)
{
	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return;

	brushless_motor[brushless].bemf_mode = 1;
}

void bldc_bemf_mode_off(bldc_t brushless)
{
	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return;

	brushless_motor[brushless].bemf_mode = 0;
}

uint16_t bldc_get_bemf_mode(bldc_t brushless)
{
	/* Capture wrong brushless number */
	if (brushless >= BLDCn) return -1;

	return brushless_motor[brushless].bemf_mode;
}

int16_t bldc_get_direction(bldc_t brushless)
{
	return brushless_motor[brushless].direction;
}

void bldc_commutation(bldc_t brushless, commutation_t commutation)
{
	switch (commutation) {
	case COMMUTATION0:
		bldc_u1_off(brushless);
		bldc_u2_off(brushless);
		bldc_v1_off(brushless);
		bldc_v2_off(brushless);
		bldc_w1_off(brushless);
		bldc_w2_off(brushless);
		break;
	case COMMUTATION1:
		bldc_u1_on(brushless);
		bldc_u2_off(brushless);
		bldc_v1_off(brushless);
		bldc_v2_on(brushless);
		bldc_w1_off(brushless);
		bldc_w2_off(brushless);
		break;
	case COMMUTATION2:
		bldc_u1_on(brushless);
		bldc_u2_off(brushless);
		bldc_v1_off(brushless);
		bldc_v2_off(brushless);
		bldc_w1_off(brushless);
		bldc_w2_on(brushless);
		break;
	case COMMUTATION3:
		bldc_u1_off(brushless);
		bldc_u2_off(brushless);
		bldc_v1_on(brushless);
		bldc_v2_off(brushless);
		bldc_w1_off(brushless);
		bldc_w2_on(brushless);
		break;
	case COMMUTATION4:
		bldc_u1_off(brushless);
		bldc_u2_on(brushless);
		bldc_v1_on(brushless);
		bldc_v2_off(brushless);
		bldc_w1_off(brushless);
		bldc_w2_off(brushless);
		break;
	case COMMUTATION5:
		bldc_u1_off(brushless);
		bldc_u2_on(brushless);
		bldc_v1_off(brushless);
		bldc_v2_off(brushless);
		bldc_w1_on(brushless);
		bldc_w2_off(brushless);
		break;
	case COMMUTATION6:
		bldc_u1_off(brushless);
		bldc_u2_off(brushless);
		bldc_v1_off(brushless);
		bldc_v2_on(brushless);
		bldc_w1_on(brushless);
		bldc_w2_off(brushless);
		break;
	default:
		/* silently fails */
		break;
	}
}

void bldc_controller_periodic_handler(void)
{
	int brushless = BLDC1;
	int BEMF_Acq = 0;
	int direction = 0;

	bldc_period_count++;

	if (bldc_period_count >= bldc_period) {

		bldc_period_count = 0;

		while (brushless < BLDCn) {

			/* choose between manual or software control inputs */
			if (brushless_motor[brushless].button_mode) {
				if (button_getstate(bldc_button_cw_pin[brushless])  == 0)
					brushless_motor[brushless].direction = 1;
				else if (button_getstate(bldc_button_ccw_pin[brushless]) == 0)
					brushless_motor[brushless].direction = -1;
				else
					brushless_motor[brushless].direction = 0;
			}
			else {
				if (labs(brushless_motor[brushless].position - brushless_motor[brushless].anchor) < 60 &&
						brushless_motor[brushless].state == BLDC_BEMF) {
					brushless_motor[brushless].startup_count = 0;
					brushless_motor[brushless].state = BLDC_SLOWING_DOWN;
				}
				if (brushless_motor[brushless].position == brushless_motor[brushless].anchor)
					brushless_motor[brushless].direction = 0;
			}

			/* brushless controller */
			direction = brushless_motor[brushless].direction;
			if (direction == 0 && brushless_motor[brushless].state != BLDC_READY)
				brushless_motor[brushless].state = BLDC_IDLE;

			switch (brushless_motor[brushless].state) {
			case BLDC_IDLE:
				brushless_motor[brushless].state = BLDC_READY;
				brushless_motor[brushless].startup_steps_count = 0;
				brushless_motor[brushless].startup_count = 0;
				bldc_commutation(brushless, COMMUTATION0);
				break;

			case BLDC_READY:
				if (direction) brushless_motor[brushless].state = BLDC_IGNITION;
				break;

			case BLDC_IGNITION:
				if (brushless_motor[brushless].startup_count == 0)
					bldc_commutation(brushless, brushless_motor[brushless].commutation);
				brushless_motor[brushless].startup_count++;
				if (brushless_motor[brushless].startup_count == brushless_motor[brushless].startup_period) {
					brushless_motor[brushless].startup_count = 0;
					brushless_motor[brushless].state = BLDC_STARTUP;
				}
				break;

			case BLDC_STARTUP:
				if (brushless_motor[brushless].startup_count == 0) {
					if (direction < 0) {
						brushless_motor[brushless].commutation--;
						brushless_motor[brushless].position--;
					}
					if (direction > 0) {
						brushless_motor[brushless].commutation++;
						brushless_motor[brushless].position++;
					}
					if (brushless_motor[brushless].commutation == COMMUTATION6+1)
						brushless_motor[brushless].commutation = COMMUTATION1;
					if (brushless_motor[brushless].commutation == COMMUTATION1-1)
						brushless_motor[brushless].commutation = COMMUTATION6;
					bldc_commutation(brushless, brushless_motor[brushless].commutation);
				}
				brushless_motor[brushless].startup_count++;
				if (brushless_motor[brushless].startup_count == brushless_motor[brushless].startup_period) {
					brushless_motor[brushless].startup_count = 0;
					if (brushless_motor[brushless].bemf_mode) brushless_motor[brushless].startup_steps_count++;
					if (brushless_motor[brushless].startup_steps_count == brushless_motor[brushless].startup_steps)
						brushless_motor[brushless].state = BLDC_TRANSITION;
				}
				break;

			case BLDC_TRANSITION:
				if (brushless_motor[brushless].commutation == COMMUTATION1) {
					if (direction < 0) brushless_motor[brushless].commutation = COMMUTATION6;
					if (direction > 0) brushless_motor[brushless].commutation = COMMUTATION2;
				}
				if (brushless_motor[brushless].commutation == COMMUTATION2) {
					if (direction < 0) brushless_motor[brushless].commutation = COMMUTATION1;
					if (direction > 0) brushless_motor[brushless].commutation = COMMUTATION3;
				}
				if (brushless_motor[brushless].commutation == COMMUTATION3) {
					if (direction < 0) brushless_motor[brushless].commutation = COMMUTATION2;
					if (direction > 0) brushless_motor[brushless].commutation = COMMUTATION4;
				}
				if (brushless_motor[brushless].commutation == COMMUTATION4) {
					if (direction < 0) brushless_motor[brushless].commutation = COMMUTATION3;
					if (direction > 0) brushless_motor[brushless].commutation = COMMUTATION5;
				}
				if (brushless_motor[brushless].commutation == COMMUTATION5) {
					if (direction < 0) brushless_motor[brushless].commutation = COMMUTATION4;
					if (direction > 0) brushless_motor[brushless].commutation = COMMUTATION6;
				}
				if (brushless_motor[brushless].commutation == COMMUTATION6) {
					if (direction < 0) brushless_motor[brushless].commutation = COMMUTATION5;
					if (direction > 0) brushless_motor[brushless].commutation = COMMUTATION1;
				}
				if (direction < 0) brushless_motor[brushless].position--;
				if (direction > 0) brushless_motor[brushless].position++;
				bldc_commutation(brushless, brushless_motor[brushless].commutation);
				brushless_motor[brushless].state = BLDC_BEMF;
				break;

			case BLDC_BEMF:
				if (brushless_motor[brushless].commutation == COMMUTATION2 || brushless_motor[brushless].commutation == COMMUTATION5)
					BEMF_Acq = analog_read(bldc_v_bemf_pin[brushless]);
				if (brushless_motor[brushless].commutation == COMMUTATION3 || brushless_motor[brushless].commutation == COMMUTATION6)
					BEMF_Acq = analog_read(bldc_u_bemf_pin[brushless]);
				if (brushless_motor[brushless].commutation == COMMUTATION4 || brushless_motor[brushless].commutation == COMMUTATION1)
					BEMF_Acq = analog_read(bldc_w_bemf_pin[brushless]);

				assert(BEMF_Acq <= 0xFFF);

				if (brushless_motor[brushless].commutation == COMMUTATION1 ||
						brushless_motor[brushless].commutation == COMMUTATION3 ||
						brushless_motor[brushless].commutation == COMMUTATION5) {
					if (direction < 0) {
						if (BEMF_Acq > zero_crossing) {
							brushless_motor[brushless].commutation--;
							brushless_motor[brushless].position--;
							if (brushless_motor[brushless].commutation == COMMUTATION6+1)
								brushless_motor[brushless].commutation = COMMUTATION1;
							if (brushless_motor[brushless].commutation == COMMUTATION1-1)
								brushless_motor[brushless].commutation = COMMUTATION6;
							bldc_commutation(brushless, brushless_motor[brushless].commutation);
						}
					}
					if (direction > 0) {if (BEMF_Acq < zero_crossing) {
							brushless_motor[brushless].commutation++;
							brushless_motor[brushless].position++;
							if (brushless_motor[brushless].commutation == COMMUTATION6+1)
								brushless_motor[brushless].commutation = COMMUTATION1;
							if (brushless_motor[brushless].commutation == COMMUTATION1-1)
								brushless_motor[brushless].commutation = COMMUTATION6;
							bldc_commutation(brushless, brushless_motor[brushless].commutation);
						}
					}
				}
				if (brushless_motor[brushless].commutation == COMMUTATION2 ||
						brushless_motor[brushless].commutation == COMMUTATION4 ||
						brushless_motor[brushless].commutation == COMMUTATION6) {
					if (direction < 0) {
						if (BEMF_Acq < zero_crossing) {
							brushless_motor[brushless].commutation--;
							brushless_motor[brushless].position--;
							if (brushless_motor[brushless].commutation == COMMUTATION6+1)
								brushless_motor[brushless].commutation = COMMUTATION1;
							if (brushless_motor[brushless].commutation == COMMUTATION1-1)
								brushless_motor[brushless].commutation = COMMUTATION6;
							bldc_commutation(brushless, brushless_motor[brushless].commutation);
						}
					}
					if (direction > 0) {
						if (BEMF_Acq > zero_crossing) {
							brushless_motor[brushless].commutation++;
							brushless_motor[brushless].position++;
							if (brushless_motor[brushless].commutation == COMMUTATION6+1)
								brushless_motor[brushless].commutation = COMMUTATION1;
							if (brushless_motor[brushless].commutation == COMMUTATION1-1)
								brushless_motor[brushless].commutation = COMMUTATION6;
							bldc_commutation(brushless, brushless_motor[brushless].commutation);
						}
					}
				}
				break;

			case BLDC_SLOWING_DOWN:
				if (brushless_motor[brushless].startup_count == 0) {
					if (direction < 0) {
						brushless_motor[brushless].commutation--;
						brushless_motor[brushless].position--;
					}
					if (direction > 0) {
						brushless_motor[brushless].commutation++;
						brushless_motor[brushless].position++;
					}
					if (brushless_motor[brushless].commutation == COMMUTATION6+1)
						brushless_motor[brushless].commutation = COMMUTATION1;
					if (brushless_motor[brushless].commutation == COMMUTATION1-1)
						brushless_motor[brushless].commutation = COMMUTATION6;
					bldc_commutation(brushless, brushless_motor[brushless].commutation);
				}
				brushless_motor[brushless].startup_count++;
				if (brushless_motor[brushless].startup_count == brushless_motor[brushless].startup_period)
					brushless_motor[brushless].startup_count = 0;
				break;

			} /* end of switch (brushless_motor[brushless].state) */

			/* next brushless */
			brushless++;

		} /* end of while (brushless < BLDCn) */
	}
}

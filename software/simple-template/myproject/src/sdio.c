/*
 * sdio.c
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

#include "sdio.h"

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void sdio_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure = {0};

	/* Enable SDIO, DMA2 and GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | SD_CARD_DETECT_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	/* Connect and configure SDIO pins: D0, D1, D2, D3, SDIO_CK and SDIO_CMD to AF12 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
	/* GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO); */
	/* GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO); */
	/* GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO); */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);

	/* Configure D0, D1, D2, D3 pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 /*| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11*/;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure SDIO_CMD pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure SDIO_CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure SD Card Detect pin */
	GPIO_InitStructure.GPIO_Pin = SD_CARD_DETECT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SD_CARD_DETECT_GPIO, &GPIO_InitStructure);

	NVIC_Configuration();
}

void sdio_deconfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure = {0};

	/* Disable SDIO clock */
	SDIO_ClockCmd(DISABLE);

	/* Set Power State to OFF */
	SDIO_SetPowerState(SDIO_PowerState_OFF);

	/* Deinit SDIO peripheral */
	SDIO_DeInit();

	/* Disable SDIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, DISABLE);

	/* Connect and configure SDIO pins: D0, D1, D2, D3, SDIO_CK and SDIO_CMD to AF12 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_MCO);
	/* GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_MCO); */
	/* GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_MCO); */
	/* GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_MCO); */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_MCO);

	/* Configure D0, D1, D2, D3 pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 /*| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11*/;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure SDIO_CMD pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure SDIO_CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

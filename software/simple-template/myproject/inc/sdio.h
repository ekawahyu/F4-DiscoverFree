/*
 * sdio.h
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

#ifndef SDIO_H_
#define SDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#define SD_CARD_DETECT_PIN				GPIO_Pin_2
#define SD_CARD_DETECT_GPIO				GPIOE
#define SD_CARD_DETECT_GPIO_CLK			RCC_AHB1Periph_GPIOE

#define SDIO_FIFO_ADDRESS				((uint32_t)0x40012C80)

/* SDIO init clock at 400kHz */
#define SDIO_INIT_CLK_DIV				((uint8_t)0x76)

/* SDIO transfer clock at 25 MHz */
#define SDIO_TRANSFER_CLK_DIV			((uint8_t)0x0)

void sdio_config(void);
void sdio_deconfig(void);

#ifdef __cplusplus
}
#endif

#endif /* SDIO_H_ */

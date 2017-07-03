/*
 * board.h
 *
 * Created on: Jan 6, 2013
 *     Author: Ekawahyu Susilo
 *
 * Copyright (c) 2013, Chongqing Aisenke Electronic Technology Co., Ltd.
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

#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <stdint.h>
#include "led.h"
#include "servo.h"
#include "bldc.h"

extern void request_to_handle(int irq, unsigned int flags);

//#define CONFIG_BOARD_HAVE_ACCELEROMETER
#define CONFIG_BOARD_HAVE_ADC
#define CONFIG_BOARD_HAVE_AUDIO
#define CONFIG_BOARD_HAVE_BUTTON
//#define CONFIG_BOARD_HAVE_DAC
#define CONFIG_BOARD_HAVE_DISK
//#define CONFIG_BOARD_HAVE_ETHERNET
#define CONFIG_BOARD_HAVE_SERVO
//#define CONFIG_BOARD_HAVE_SPI
#define CONFIG_BOARD_HAVE_USART
//#define CONFIG_BOARD_HAVE_WIFI
#define CONFIG_BOARD_HAVE_LED
#define CONFIG_BOARD_HAVE_BLDC

#define NONMASKABLE_IRQn			NonMaskableInt_IRQn
#define LOWEST_IRQn					NONMASKABLE_IRQn
#define SYSTICK_IRQn				SysTick_IRQn
#define COM1_IRQn					USART3_IRQn
#define DISK1_IRQn					SDIO_IRQn

#define COM_TXE_IRQ					0x0001
#define COM_RXNE_IRQ				0x0002

int audio1_device_init(void);
char audio1_device_read(void);
int audio1_device_read_volume(void);
int audio1_device_soft_reset(void);
int audio1_device_write(char *ptr);
int audio1_device_write_volume(int volume);

uint32_t cpu_device_getstackpointer(void);

uint32_t rtc_device_gettime(void);
void rtc_device_settime(uint32_t now);

void com1_device_deinit(void);
void com1_device_disable_irq(unsigned int flag);
void com1_device_enable_irq(unsigned int flag);
int com1_device_init(void);
int com1_device_is_rx_empty(void);
int com1_device_is_tx_empty(void);
char com1_device_receive(void);
void com1_device_send(char data);

void clock_device_init(void);
int systick_device_config(uint32_t clocks_per_sec);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H_ */

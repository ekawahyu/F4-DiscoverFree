/*
 * init.h
 *
 * Created on: Dec 12, 2012
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

#ifndef INIT_H_
#define INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "compiler.h"

#include "clock.h"
#include "led.h"
#include "adc.h"
#include "usart.h"
#include "button.h"
#include "vs1053b.h"
#include "servo.h"
#include "ff.h"

extern FRESULT res;
extern FILINFO fno;
extern FIL fil;
extern DIR dir;
extern FATFS fs32;

typedef enum {
	ACCEL_X,
	ACCEL_Y,
	ACCEL_Z,
	ACCELn
} accel_t;

void accelerometer_config(void);
uint16_t accelerometer_read(accel_t accel);
void audio_config(void);
void audio_soft_reset(void);
void audio_set_volume(uint16_t volume);
void audio_write_data(uint8_t * buffer);
void system_init(void);

#ifdef __cplusplus
}
#endif

#endif /* INIT_H_ */

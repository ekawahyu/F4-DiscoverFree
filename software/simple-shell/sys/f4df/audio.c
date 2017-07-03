/*
 * audio.c
 *
 * Created on: Jul 20, 2012
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

#include <string.h>
#include <stdint.h>
#include "vs1053b.h"

int audio1_device_init(void)
{
	vs1053b_config();
	vs1053b_soft_reset();
	vs1053b_set_volume(0x2020);

	return 0;
}

char audio1_device_read(void)
{
	/* not implemented */

	return 0;
}

int audio1_device_read_volume(void)
{
	uint16_t vol;

	vol = vs1053b_get_volume() >> 8;
	vol = 255 - vol;

	return (int)vol;
}

int audio1_device_soft_reset(void)
{
	vs1053b_soft_reset();

	return 0;
}

int audio1_device_write(char *ptr)
{
	vs1053b_write_data((void*)(ptr));

	return 0;
}

int audio1_device_write_volume(int volume)
{
	uint16_t vol;

	if (volume > 255) volume = 255;
	if (volume < 0) volume = 0;

	vol = (uint16_t)volume;
	vol = 255 - vol;
	vol = (vol << 8) | vol;

	vs1053b_set_volume(vol);

	return 0;
}

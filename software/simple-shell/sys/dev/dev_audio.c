/*
 * dev_audio.c
 *
 * Created on: Jul 18, 2012
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
#include "system.h"
#include "board.h"
#include "ioctl.h"

#ifdef CONFIG_BOARD_HAVE_AUDIO
int audio1_init(int fd)
{
	static uint8_t init_done = 0;

	if (init_done) return 0;
	audio1_device_init();
	init_done = 1;

	return 0;
}

int audio1_read(int fd, char *ptr, size_t len)
{
	size_t counter = 0;

	if (device_table_list[fd]->is_open)

		counter = audio1_device_read();

	else {
		errno = ENODEV;
		return -1;
	}

	return counter;
}

int audio1_write(int fd, char *ptr, size_t len)
{
	/* FIXME device driver only supports 32 bytes write at once */
	if (len != 32) {
		errno = EOPNOTSUPP;
		return -1;
	}

	if (device_table_list[fd]->is_open)

		audio1_device_write(ptr);

	else {
		errno = ENODEV;
		return -1;
	}

	return len;
}

int audio1_ioctl(int fd, int cmd, void *ptr)
{
	int * pint = (int*) ptr;

	if (device_table_list[fd]->is_open) {

		switch (cmd) {

		case IOCTL_SOUND_MIXER_WRITE_VOLUME:
			return audio1_device_write_volume(*pint);

		case IOCTL_SOUND_MIXER_READ_VOLUME:
			*pint =  audio1_device_read_volume();
			return *pint;

		case IOCTL_SOUND_DEVICE_SOFT_RESET:
			return audio1_device_soft_reset();
		}
	}
	else {
		errno = ENODEV;
		return -1;
	}

	return 0;
}

device_table_entry_t audio1_driver = {
		.name          = "/dev/audio1",
		.device_type   = "AUDIO1",
		.device_number = 0,
		.is_open       = 0,
		.open          = stdev_open,
		.close         = stdev_close,
		.write         = audio1_write,
		.read          = audio1_read,
		.init          = audio1_init,
		.ioctl         = audio1_ioctl
};
#endif

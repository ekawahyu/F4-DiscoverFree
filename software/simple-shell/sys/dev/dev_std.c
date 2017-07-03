/*
 * dev_std.c
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
#include "system.h"

int stdev_close(int fd)
{
	if (fd < 0) {
		errno = EBADF;
		return -1;
	}

	if ((STDOUT_FILENO == fd) ||
			(STDERR_FILENO == fd) ||
				(STDIN_FILENO == fd)) {
		errno = EBUSY;
		return -1;
	}

	device_table_list[fd]->is_open = 0;

	return 0;
}

int stdev_open(char *name, int flags, int mode)
{
	int which_dev = 0;
	int fd = -2;

	/* search for file/device in the list */
	do {
		if (strcmp(device_table_list[which_dev]->name, name) == 0) {
			fd = which_dev;
			break;
		}
		which_dev++;
	} while(device_table_list[which_dev]);

	/* if we found the requested file/device, check if it is busy */
	if (device_table_list[which_dev] && device_table_list[which_dev]->is_open) {
		fd = -1;
		errno = ETXTBSY;
		return fd;
	}

	if (fd == -2) {
		fd = -1;
		errno = ENODEV;
		return fd;
	}

	device_table_list[fd]->is_open = 1;

	return fd;
}

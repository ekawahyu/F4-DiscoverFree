/*
 * dev_null.c
 *
 * Created on: Jun 20, 2012
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
#include "debug.h"
#include "system.h"

DEBUGMSG_VAR(debug_count);

int null_write(int fd, char *ptr, size_t len);
int null_read(int fd, char *ptr, size_t len);
int null_init(int fd);
int null_ioctl(int fd, int cmd, void *ptr);
int null_register(void);

device_table_entry_t null_driver = {
		.name          = "/dev/null",
		.device_type   = "NULL",
		.device_number = 0,
		.is_open       = 0,
		.open          = stdev_open,
		.close         = stdev_close,
		.write         = null_write,
		.read          = null_read,
		.init          = null_init,
		.ioctl         = null_ioctl
};

int null_write(int fd, char *ptr, size_t len)
{
	size_t counter = len;

	if (device_table_list[fd]->is_open) {
		while(counter-- > 0) {
			ptr++; /* read pointer but do nothing */
		}
	}
	else {
		errno = ENODEV;
		return -1;
	}

	return len;
}

int null_read(int fd, char *ptr, size_t len)
{
	int i;
	size_t counter = 0;

	if (device_table_list[fd]->is_open) {
		for (i = 0; i < len; i++) {
			*ptr++ = 0; /* filling buffer with all 0 */
			counter++;
		}
	}
	else {
		errno = ENODEV;
		return -1;
	}

	return counter;
}

int null_init(int fd)
{
	return 0; /* always successful */
}

int null_ioctl(int fd, int cmd, void *ptr)
{
	return 0; /* always successful */
}

int null_register(void)
{
	int result;

	/* registering null device driver */
	DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:registering %s\n", null_driver.name);
	DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_type %s\n", null_driver.device_type);
	DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_no.  %i\n", null_driver.device_number);
	result = register_chrdev(NOT_ASSIGNED_YET, NULL, &null_driver);
	if (result > 0) {
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:assigned to fd#%i\n", result);
	}
	else {
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:***not successful (\"%s\")\n", null_driver.name);
	}

	return 0;
}

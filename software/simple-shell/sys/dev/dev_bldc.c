/*
 * dev_bldc.c
 *
 * Created on: Jun 21, 2012
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

#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "system.h"
#include "board.h"

#ifdef CONFIG_BOARD_HAVE_BLDC
DEBUGMSG_VAR(debug_count);

int bldc_write(int fd, char *ptr, size_t len);
int bldc_read(int fd, char *ptr, size_t len);
int bldc_init(int fd);
int bldc_ioctl(int fd, int cmd, void *ptr);
int bldc_register(void);

static char dev_name[SYSDEV_NAME_LEN * BLDCn];
static char dev_type[SYSDEV_TYPE_LEN * BLDCn];

device_table_entry_t bldc_driver[BLDCn] = {
		[0 ... (BLDCn-1)] = {
		.name          = NULL,
		.device_type   = NULL,
		.device_number = 0,
		.is_open       = 0,
		.open          = stdev_open,
		.close         = stdev_close,
		.write         = bldc_write,
		.read          = bldc_read,
		.init          = bldc_init,
		.ioctl         = bldc_ioctl
		}
};

int bldc_write(int fd, char *ptr, size_t len)
{
	char * mcommand;
	char * token;
	int status = 0;

	if (device_table_list[fd]->is_open) {

		/* parsing command */
		mcommand = malloc(strlen(ptr)+1);
		memset(mcommand, 0, strlen(ptr)+1);
		memcpy(mcommand, ptr, strlen(ptr));

		token = strtok((char*) mcommand, " ");

		if (*token == 'L') {
			token = strtok(NULL, " ");
			status = atoi(token);
			if (status)
				led_on(device_table_list[fd]->device_number);
			else
				led_off(device_table_list[fd]->device_number);
		}

		if (*token == 'T')
			led_toggle(device_table_list[fd]->device_number);

		free(mcommand);
	}

	else {
		errno = ENODEV;
		return -1;
	}

	return len;
}

/* not implemented at the moment */
int bldc_read(int fd, char *ptr, size_t len)
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

int bldc_init(int fd)
{
	if (device_table_list[fd]->device_number >= BLDCn) return -1;

	bldc_config(device_table_list[fd]->device_number);

	return 0;
}

int bldc_ioctl(int fd, int cmd, void *ptr)
{
	return 0;
}

int bldc_register(void)
{
	int bldc = 0;
	int result = 0;

	memset(dev_name,0,sizeof(dev_name));
	memset(dev_type,0,sizeof(dev_type));

	while (bldc < BLDCn) {

		/* prepare the device name and type, ordered by number */
		sprintf(dev_name + (SYSDEV_NAME_LEN * bldc), "/dev/bldc%d", bldc);
		sprintf(dev_type + (SYSDEV_TYPE_LEN * bldc), "BLDC%d", bldc);

		/* initialize device driver structure */
		bldc_driver[bldc].name          = dev_name + (SYSDEV_NAME_LEN * bldc);
		bldc_driver[bldc].device_type   = dev_type + (SYSDEV_TYPE_LEN * bldc);
		bldc_driver[bldc].device_number = bldc;
		//bldc_driver[bldc].open          = bldc_driver[0].open;
		//bldc_driver[bldc].close         = bldc_driver[0].close;
		//bldc_driver[bldc].write         = bldc_driver[0].write;
		//bldc_driver[bldc].read          = bldc_driver[0].read;
		//bldc_driver[bldc].init          = bldc_driver[0].init;
		//bldc_driver[bldc].ioctl         = bldc_driver[0].ioctl;

		/* registering BLDC device driver */
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:registering %s\n", bldc_driver[bldc].name);
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_type %s\n", bldc_driver[bldc].device_type);
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_no.  %i\n", bldc_driver[bldc].device_number);
		result = register_chrdev(NOT_ASSIGNED_YET, NULL, &bldc_driver[bldc]);
		if (result > 0) {
			DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:assigned to fd#%i\n", result);
		}
		else {
			DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:***not successful (\"%s\")\n", bldc_driver[bldc].name);
			break;
		}

		/* next BLDC */
		bldc++;
	}

	return 0;
}
#else
int bldc_register(void)
{
	return 0;
}
#endif

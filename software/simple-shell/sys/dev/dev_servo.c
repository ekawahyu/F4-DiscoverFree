/*
 * dev_servo.c
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

#ifdef CONFIG_BOARD_HAVE_SERVO
DEBUGMSG_VAR(debug_count);

int servo_write(int fd, char *ptr, size_t len);
int servo_read(int fd, char *ptr, size_t len);
int servo_init(int fd);
int servo_ioctl(int fd, int cmd, void *ptr);
int servo_register(void);

static char dev_name[SYSDEV_NAME_LEN * SERVOn];
static char dev_type[SYSDEV_TYPE_LEN * SERVOn];

device_table_entry_t servo_driver[SERVOn] = {
		[0 ... (SERVOn-1)] = {
		.name          = NULL,
		.device_type   = NULL,
		.device_number = 0,
		.is_open       = 0,
		.open          = stdev_open,
		.close         = stdev_close,
		.write         = servo_write,
		.read          = servo_read,
		.init          = servo_init,
		.ioctl         = servo_ioctl
		}
};

int servo_write(int fd, char *ptr, size_t len)
{
	char * mcommand;
	char * token;
	int position = 0;

	if (device_table_list[fd]->is_open) {

		/* parsing command */
		mcommand = malloc(strlen(ptr)+1);
		memset(mcommand, 0, strlen(ptr)+1);
		memcpy(mcommand, ptr, strlen(ptr));

		token = strtok((char*) mcommand, " ");
		if (*token == 'P') {
			token = strtok(NULL, " ");
			position = atoi(token);
			servo_move(device_table_list[fd]->device_number, position);
		}

		free(mcommand);
	}

	else {
		errno = ENODEV;
		return -1;
	}

	return len;
}

/* not implemented at the moment */
int servo_read(int fd, char *ptr, size_t len)
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

int servo_ioctl(int fd, int cmd, void *ptr)
{
	return 0;
}

int servo_init(int fd)
{
	if (device_table_list[fd]->device_number >= SERVOn) return -1;

	servo_config(device_table_list[fd]->device_number);

	return 0;
}

int servo_register(void)
{
	int servo = 0;
	int result = 0;

	memset(dev_name,0,sizeof(dev_name));
	memset(dev_type,0,sizeof(dev_type));

	while (servo < SERVOn) {

		/* prepare the device name and type, ordered by number */
		sprintf(dev_name + (SYSDEV_NAME_LEN * servo), "/dev/servo%d", servo);
		sprintf(dev_type + (SYSDEV_TYPE_LEN * servo), "SERVO%d", servo);

		/* initialize device driver structure */
		servo_driver[servo].name          = dev_name + (SYSDEV_NAME_LEN * servo);
		servo_driver[servo].device_type   = dev_type + (SYSDEV_TYPE_LEN * servo);
		servo_driver[servo].device_number = servo;
		//servo_driver[servo].open          = servo_driver[0].open;
		//servo_driver[servo].close         = servo_driver[0].close;
		//servo_driver[servo].write         = servo_driver[0].write;
		//servo_driver[servo].read          = servo_driver[0].read;
		//servo_driver[servo].init          = servo_driver[0].init;
		//servo_driver[servo].ioctl         = servo_driver[0].ioctl;

		/* registering SERVO device driver */
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:registering %s\n", servo_driver[servo].name);
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_type %s\n", servo_driver[servo].device_type);
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_no.  %i\n", servo_driver[servo].device_number);
		result = register_chrdev(NOT_ASSIGNED_YET, NULL, &servo_driver[servo]);
		if (result > 0) {
			DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:assigned to fd#%i\n", result);
		}
		else {
			DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:***not successful (\"%s\")\n", servo_driver[servo].name);
			break;
		}

		/* next SERVO */
		servo++;
	}

	return 0;
}
#else
int servo_register(void)
{
	return 0;
}
#endif


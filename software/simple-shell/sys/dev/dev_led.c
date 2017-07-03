/*
 * dev_led.c
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

#ifdef CONFIG_BOARD_HAVE_LED
DEBUGMSG_VAR(debug_count);

int led_write(int fd, char *ptr, size_t len);
int led_read(int fd, char *ptr, size_t len);
int led_init(int fd);
int led_ioctl(int fd, int cmd, void *ptr);
int led_register(void);

static char dev_name[SYSDEV_NAME_LEN * LEDn];
static char dev_type[SYSDEV_TYPE_LEN * LEDn];

device_table_entry_t led_driver[LEDn] = {
		[0 ... (LEDn-1)] = {
		.name          = NULL,
		.device_type   = NULL,
		.device_number = 0,
		.is_open       = 0,
		.open          = stdev_open,
		.close         = stdev_close,
		.write         = led_write,
		.read          = led_read,
		.init          = led_init,
		.ioctl         = led_ioctl
		}
};

int led_write(int fd, char *ptr, size_t len)
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
int led_read(int fd, char *ptr, size_t len)
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

int led_ioctl(int fd, int cmd, void *ptr)
{
	return 0;
}

int led_init(int fd)
{
	if (device_table_list[fd]->device_number >= LEDn) return -1;

	led_config(device_table_list[fd]->device_number);

	return 0;
}

int led_register(void)
{
	int led = 0;
	int result = 0;

	memset(dev_name,0,sizeof(dev_name));
	memset(dev_type,0,sizeof(dev_type));

	while (led < LEDn) {

		/* prepare the device name and type, ordered by number */
		sprintf(dev_name + (SYSDEV_NAME_LEN * led), "/dev/led%d", led);
		sprintf(dev_type + (SYSDEV_TYPE_LEN * led), "LED%d", led);

		/* initialize device driver structure */
		led_driver[led].name          = dev_name + (SYSDEV_NAME_LEN * led);
		led_driver[led].device_type   = dev_type + (SYSDEV_TYPE_LEN * led);
		led_driver[led].device_number = led;
		//led_driver[led].open          = led_driver[0].open;
		//led_driver[led].close         = led_driver[0].close;
		//led_driver[led].write         = led_driver[0].write;
		//led_driver[led].read          = led_driver[0].read;
		//led_driver[led].init          = led_driver[0].init;
		//led_driver[led].ioctl         = led_driver[0].ioctl;

		/* registering LED device driver */
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:registering %s\n", led_driver[led].name);
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_type %s\n", led_driver[led].device_type);
		DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:device_no.  %i\n", led_driver[led].device_number);
		result = register_chrdev(NOT_ASSIGNED_YET, NULL, &led_driver[led]);
		if (result > 0) {
			DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:assigned to fd#%i\n", result);
		}
		else {
			DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "dev:***not successful (\"%s\")\n", led_driver[led].name);
			break;
		}

		/* next LED */
		led++;
	}

	return 0;
}

void led_blink(void)
{
	led_config(LED1);

	led_on(LED1);
	usleep(10000);

	led_off(LED1);
	usleep(1000000);
}
#else
int led_register(void)
{
	return 0;
}

void led_blink(void)
{

}
#endif

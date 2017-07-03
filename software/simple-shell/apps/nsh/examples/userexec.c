/*
 * userexec.c
 *
 * Created on: Jul 12, 2012
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
#include <stdlib.h>
#include <fcntl.h>
#include "system.h"

int cursor(int argc, char * argv[])
{
	int x, y;

	if (argc < 3) {
		printf("cursor: ROW and COLUMN required\n");
		return -1;
	}

	x = atoi(argv[1]);
	y = atoi(argv[2]);

	printf("\033[%d;%dH", x, y);

	return 0;
}

int nop(int argc, char * argv[])
{
	return 0;
}

int welcome(int argc, char * argv[])
{
	char file_buffer[128];
	int fd, br;

	fd = open("welcome.txt", O_RDONLY, 0);

	if (fd == -1) {
		if (errno == ENODEV) fprintf(stderr, "welcome: open failed\n");
		if (errno == ETXTBSY) fprintf(stderr, "welcome: file busy\n");
		if (errno == ENOENT) fprintf(stderr, "welcome: no file/dir\n");
	}
	else {
		do {
			br = read(fd, file_buffer, sizeof(file_buffer)-1);
			file_buffer[br] = '\0';
			if (br < 0) fprintf(stderr, "welcome: read failed!\n");
			if (br > 0) printf("%s", file_buffer);
		} while (br);

		fd = close(fd);
		if (fd < 0) fprintf(stderr, "welcome: close failed!\n");
	}

	return fd;
}

int led(int argc, char * argv[])
{
	int led_handle;
	int key = 0;
	int count;

	led_handle = open("/dev/led1", O_RDWR);

	if (led_handle >= 0) {
		printf("led_handle = %i\n", led_handle);
	}
	else {
		if (errno == ENODEV) printf("no device\n");
		if (errno == ETXTBSY) printf("device busy\n");
		return -1;
	}

	for (count = 0; count < 5; count++) {
		write(led_handle, "T", 1);
		if (keypressed()) {
			key = getchar();
			if (key == KEY_ETX) break; /* Ctrl-C */
		}
		Sleep(1000);
	};

	close(led_handle);

	return 0;
}

int servo(int argc, char * argv[])
{
	int servo_handle;

	servo_handle = open("/dev/servo0", O_RDWR);

	if (servo_handle >= 0) {
		printf("servo_handle = %i\n", servo_handle);
	}
	else {
		if (errno == ENODEV) printf("no device\n");
		if (errno == ETXTBSY) printf("device busy\n");
		return -1;
	}

	write(servo_handle, "P 200", 5);

	close(servo_handle);

	return 0;
}

/*
 * open.c
 *
 * Created on: Jul 3, 2012
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
#include <string.h>
#include <assert.h>
#include "system.h"

/* Open a file/device */
int _open(char *name, int flags, int mode)
{
	int which_dev = 0;
	int fd = -1;
	char devname[25];
	char * p1 = NULL;
	char * p2 = NULL;
	char * mname;

	if( (name == 0) || (*name == '\0')) {
		errno = ENOENT;
		return -1;
	}

	memset(devname, 0, 25);

	/* truncate the name to get /dev/aaa only */
	mname = calloc(strlen(name)+1, sizeof(char));
	assert(mname != NULL);
	memcpy(mname, name, strlen(name));
	p1 = strtok((char*) mname, "/");
	p2 = strtok(NULL, "/");
	if (p2) sprintf(devname, "/%s/%s", p1, p2);
	free(mname);

	/* if the leading path is not /dev
	 * redirect to /dev/sda1
	 */
	if (strcmp(p1, "dev") != 0) {
		strncpy(devname, "/dev/sda1", 10);
	}

	/* search for "file" in device_table_list[].name */
	do {
		if (strcmp(device_table_list[which_dev]->name, devname) == 0) {
			fd = which_dev;
			break;
		}
		which_dev++;
	} while(device_table_list[which_dev]);

	/* if we found the requested file/device,
	 * then invoke the device's open() method
	 */
	if (fd == -1) {
		errno = ENODEV;
		return fd;
	}

	if (device_table_list[fd]->open)
		return device_table_list[fd]->open(name, flags, mode);
	else {
		errno = ENOSYS;
		return -1;
	}
}

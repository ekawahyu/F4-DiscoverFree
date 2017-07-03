/*
 * dev_disk.c
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include "system.h"
#include "board.h"
#include "interrupt.h"

#ifdef CONFIG_BOARD_HAVE_DISK
#include "ff.h"

static FATFS Fatfs;
static FIL fsrc;

void unix2dos(char * name)
{
	char * pch = NULL;

	pch = strchr(name, '/');
	while (pch) {
		*pch = '\\';
		pch = strchr(name, '/');
	}
}

int sda1_open(char *name, int flags, int mode)
{
	int translate_flags = 0;
	int which_dev = 0;
	int fd = -2;
	char * mname;

	/* search for file/device in the list */
	do {
		if (strcmp(device_table_list[which_dev]->name, disk1_driver.name) == 0) {
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

	memset(&fsrc,0,sizeof(FIL));

	/* we need to make a copy of name (could be a const declared) */
	mname = calloc(strlen(name)+1, sizeof(char));
	assert(mname != NULL);
	memcpy(mname, name, strlen(name));

	unix2dos(mname);

	if (((flags & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC)) && (flags & (O_RDWR | O_WRONLY)))
		translate_flags = FA_CREATE_ALWAYS;
	else if ((flags & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL))
		translate_flags = FA_OPEN_EXISTING;
	else if ((flags & O_CREAT) == O_CREAT)
		translate_flags = FA_OPEN_ALWAYS;
	else if ((flags == O_RDONLY) || (flags == O_WRONLY) || (flags == O_RDWR))
		translate_flags = FA_OPEN_EXISTING;
	else {
		errno = EINVAL;
		return -1;
	}

	if ((flags & O_ACCMODE) == O_RDONLY)
		translate_flags |= FA_READ;
	else if ((flags & O_ACCMODE) == O_WRONLY)
		translate_flags |= FA_WRITE;
	else if ((flags & O_ACCMODE) == O_RDWR)
		translate_flags |= (FA_READ | FA_WRITE);
	else {
		errno = EINVAL;
		return -1;
	}

	if (f_open(&fsrc, mname, translate_flags) == FR_OK) {
		device_table_list[fd]->is_open = 1;
	}
	else {
		fd = -1;
		errno = ENOENT;
	}

	free(mname);

	return fd;
}

int sda1_close(int fd)
{
	if (f_close(&fsrc) == FR_OK) {
		device_table_list[fd]->is_open = 0;
		return 0;
	}

	return -1;
}

int sda1_write(int fd, char *ptr, size_t len)
{
	size_t bytewritten;

	if (device_table_list[fd]->is_open) {
		if (f_write(&fsrc, ptr, len, &bytewritten) == FR_OK){}
		else bytewritten = -1;
	}
	else {
		errno = ENODEV;
		bytewritten = -1;
	}

	return bytewritten;
}

int sda1_read(int fd, char *ptr, size_t len)
{
	size_t byteread;

	if (device_table_list[fd]->is_open) {
		/* read from disk and put in the buffer ptr */
		if (f_read(&fsrc, ptr, len, &byteread) == FR_OK){}
		else byteread = -1;
	}
	else {
		errno = ENODEV;
		byteread = -1;
	}

	return byteread;
}

int sda1_init(int fd)
{
	static uint8_t init_done = 0;

	if (init_done) return 0;
	memset(&Fatfs, 0, sizeof(FATFS));
	f_mount(0, &Fatfs);
	init_done = 1;

	return 0;
}

int sda1_ioctl(int fd, int cmd, void *ptr)
{
	return 0;
}

void sda1_irq_handler(unsigned int flag)
{
	switch (flag) {
	case 0:
		break;
	default:
		/* wrong flag, silently fails */
		return;
	}
}

device_table_entry_t disk1_driver = {
		.name          = "/dev/sda1",
		.device_type   = "SDCard",
		.device_number = 0,
		.is_open       = 0,
		.open          = sda1_open,
		.close         = sda1_close,
		.write         = sda1_write,
		.read          = sda1_read,
		.init          = sda1_init,
		.ioctl         = sda1_ioctl
};

irq_hook_t disk1_irq_hook = {
		.name        = "/irq/sda1",
		.id          = DISK1_IRQn,
		.handler     = sda1_irq_handler,
		.disable_irq = NULL,
		.enable_irq  = NULL,
		.flags       = 0x00
};

/* TODO open and close directory */

long _tell(int fd)
{
	FIL *pfsrc = &fsrc;

	return pfsrc->fptr;
}

long get_file_size(int fd)
{
	FIL *pfsrc = &fsrc;

	return pfsrc->fsize;
}

#endif

/* Set a position in a file */
long _lseek(int fd, long offset, int origin)
{
	if ((STDOUT_FILENO == fd) ||
			(STDERR_FILENO == fd) ||
				(STDIN_FILENO == fd)) {
		return 0;
	}
	else { // FIXME this should work for both files/devices that support lseek!
#ifdef CONFIG_BOARD_HAVE_DISK
		if( origin == SEEK_SET)
		{

		   if( f_lseek(&fsrc,offset) == FR_OK)
		   {
			   return _tell(fd);
		   }

		}
		else
		if( origin == SEEK_CUR)
		{
		   if( f_lseek(&fsrc,_tell(fd)+offset) == FR_OK)
		   {
			  return _tell(fd);
		   }
		}
		else
		if( origin == SEEK_END)
		{
			return _tell(fd);
		}

		return (long) -1;
#endif
	}

	return 0;
}

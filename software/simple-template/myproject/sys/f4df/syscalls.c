/*
 * syscalls.c
 *
 * Created on: Dec 1, 2012
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

#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "led.h"
#include "usart.h"
#include "usbd_cdc_vcp.h"

#undef errno
extern int errno;

#ifdef __caddr_t_required_
typedef char * caddr_t;
#endif

#ifdef __stat_struct_required_
struct stat
{
	unsigned long int st_mode;
};
#endif

#ifndef ENOMEM
#define	ENOMEM 12 /* Not enough core */
#endif

#ifndef EBADF
#define EBADF 9 /* Bad file number */
#endif

#ifndef EINVAL
#define EINVAL 22 /* Invalid argument */
#endif

#ifndef _IFCHR
#define _IFCHR 0020000 /* character special */
#endif

#ifndef S_IFCHR
#define S_IFCHR _IFCHR
#endif

int _write(int fd, char *ptr, size_t len)
{
	size_t counter = len;

	if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		/* do nothing */
	}
	else return -1;

	while(counter-- > 0) { // Send the character from the buffer to UART
		while (!usart_is_tx_empty());
		usart_send(*ptr);
		//VCP_put_char(*ptr);

		ptr++;
	}

	return len;
}

caddr_t _sbrk(int incr)
{
	extern char _ebss; // Defined by the linker
	static char *heap_end;
	char *prev_heap_end;
	char *stack;

	if (heap_end == 0) heap_end = &_ebss;

	prev_heap_end = heap_end;

	stack = (char*) __get_MSP();

	if (heap_end + incr >  stack)
	{
		_write (STDERR_FILENO, "Heap and stack collision\n", 25);
		errno = ENOMEM;
		return  (caddr_t) -1;
		//abort ();
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

void _exit(int status)
{
	volatile uint32_t delay;

	led_config(LED1);

	_write (STDERR_FILENO, "exit", 4);

	while (1)
	{
		led_on(LED1);
		delay = 0x0007FFFF;
		while(delay--);
		led_off(LED1);
		delay = 0x007FFFFF;
		while(delay--);
	}
}

int _read(int fd, char *ptr, size_t len)
{
	int i;
	size_t counter = 0;
	uint8_t buf;

	if(fd == STDIN_FILENO) {
		/* do nothing */
	}
	else return -1;

	for (i = 0; i < len; i++) { // Get characters from the UART
		while (usart_is_rx_empty());
		*ptr++ = usart_receive();
		//while (!VCP_get_char(&buf));
		//*ptr++ = (char) buf;
		counter++;
	}

	return counter;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;

	return 0;
}

int _isatty(int file)
{
	switch (file)
	{
		case STDOUT_FILENO:
		case STDERR_FILENO:
		case STDIN_FILENO:
			return 1;
		default:
			errno = EBADF;
			return 0;
	}
}

#define __MY_PID	1
int _getpid(void)
{
	return  __MY_PID;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;

	return -1;
}

/*
 * dev_tty.c
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

#include <string.h>
#include <signal.h>
#include "system.h"
#include "board.h"
#include "interrupt.h"
#include "ioctl.h"

#ifdef CONFIG_BOARD_HAVE_USART
#define USART_BUFFER_LEN		1024

static char usart1_tx_buffer[USART_BUFFER_LEN];
static char * volatile pusart1_tx_head = usart1_tx_buffer;
static char * volatile pusart1_tx_tail = usart1_tx_buffer;

static char usart1_rx_buffer[USART_BUFFER_LEN];
static char * volatile pusart1_rx_head = usart1_rx_buffer;
static char * volatile pusart1_rx_tail = usart1_rx_buffer;

void com1_disable_irq(int irq, unsigned int flag)
{
	com1_device_disable_irq(flag);
}

void com1_enable_irq(int irq, unsigned int flag)
{
	com1_device_enable_irq(flag);
}

int com1_init(int fd)
{
	static uint8_t init_done = 0;

	if (init_done) return 0;
	com1_device_init();
	init_done = 1;

	return 0;
}

int com1_read(int fd, char *ptr, size_t len)
{
	int i;
	size_t counter = 0;

	if (device_table_list[fd]->is_open) {

		/* if COM1 interrupt is used */
		if (irq_is_enabled(COM1_IRQn, COM_RXNE_IRQ)) {
			for (i = 0; i < len; i++) {
				while (pusart1_rx_head == pusart1_rx_tail);
				*ptr++ = *pusart1_rx_head++;
				if (pusart1_rx_head > &usart1_rx_buffer[USART_BUFFER_LEN - 1])
					pusart1_rx_head = usart1_rx_buffer;
				counter++;
			}
		}
		/* polling COM1 flag */
		else {
			for (i = 0; i < len; i++) {
				while (com1_device_is_rx_empty());
				*ptr++ = com1_device_receive();
				counter++;
			}
		}
	}
	else {
		errno = ENODEV;
		return -1;
	}

	return counter;
}

int com1_write(int fd, char *ptr, size_t len)
{
	size_t counter = len;

	if (device_table_list[fd]->is_open) {

		/* if COM1 interrupt is used */
		if (irq_is_enabled(COM1_IRQn, COM_TXE_IRQ)) {
			while (counter-- > 0) {
				*pusart1_tx_tail++ = *ptr++;
				if (pusart1_tx_tail == pusart1_tx_head) pusart1_tx_tail--;
				if (pusart1_tx_tail > &usart1_tx_buffer[USART_BUFFER_LEN - 1])
					pusart1_tx_tail = usart1_tx_buffer;
				enable_irq(COM1_IRQn, COM_TXE_IRQ);
			}
		}
		/* polling COM1 flag */
		else {
			while(counter-- > 0) {
				while (!com1_device_is_tx_empty());
				com1_device_send(*ptr);
				ptr++;
			}
		}
	}
	else {
		errno = ENODEV;
		return -1;
	}

	return len;
}

int com1_ioctl(int fd, int cmd, void *ptr)
{
	int count = 0;
	int * pint = (int*) ptr;

	if (device_table_list[fd]->is_open) {

		switch (cmd) {
		case IOCTL_NUM_OF_CHARS_WAITING:

			if (pusart1_rx_head == pusart1_rx_tail)
				count = 0;
			else if (pusart1_rx_head < pusart1_rx_tail)
				count = pusart1_rx_tail - pusart1_rx_head;
			else
				count = USART_BUFFER_LEN - (pusart1_rx_head - pusart1_rx_tail);

			*pint = count;
			return count;
		}
	}
	else {
		errno = ENODEV;
		return -1;
	}

	return 0;
}

void com1_irq_handler(unsigned int flag)
{
	int signal_pending = 0;

	switch (flag) {
	case COM_TXE_IRQ:
		if (pusart1_tx_head != pusart1_tx_tail)
			com1_device_send(*pusart1_tx_head++);
		else
			disable_irq(COM1_IRQn, COM_TXE_IRQ);
		if (pusart1_tx_head > &usart1_tx_buffer[USART_BUFFER_LEN - 1])
			pusart1_tx_head = usart1_tx_buffer;
		break;
	case COM_RXNE_IRQ:
		*pusart1_rx_tail++ = com1_device_receive();
		if (*(pusart1_rx_tail-1) == KEY_ETX) signal_pending = 1;
		if (pusart1_rx_tail == pusart1_rx_head) pusart1_rx_tail--;
		if (pusart1_rx_tail > &usart1_rx_buffer[USART_BUFFER_LEN - 1])
			pusart1_rx_tail = usart1_rx_buffer;
		break;
	default:
		/* wrong flag, silently fails */
		return;
	}

	if (signal_pending) raise(SIGINT);
}

device_table_entry_t com1_driver = {
		.name          = "/dev/ttyS1",
		.device_type   = "COM1",
		.device_number = 0,
		.is_open       = 0,
		.open          = stdev_open,
		.close         = stdev_close,
		.write         = com1_write,
		.read          = com1_read,
		.init          = com1_init,
		.ioctl         = com1_ioctl
};

irq_hook_t com1_irq_hook = {
		.name        = "/irq/com1",
		.id          = COM1_IRQn,
		.handler     = com1_irq_handler,
		.disable_irq = com1_disable_irq,
		.enable_irq  = com1_enable_irq,
		.flags       = 0x00
};
#endif

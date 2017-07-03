/*
 * system.c
 *
 * Created on: Jan 6, 2013
 *     Author: Ekawahyu Susilo
 *
 * Copyright (c) 2013, Chongqing Aisenke Electronic Technology Co., Ltd.
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

#include "compiler.h"
#include "debug.h"
#include "board.h"
#include "system.h"
#include "interrupt.h"
//#include "console.h"

int led_register(void);
int servo_register(void);
int bldc_register(void);
int null_register(void);

irq_hook_t systick_irq_hook;
irq_hook_t com1_irq_hook;
irq_hook_t com2_irq_hook;

DEBUGMSG_VAR(debug_count);

device_table_entry_t *device_table_list[NR_DEVICE_DRIVERS] = { NULL };

int register_chrdev(int fd, char *name, device_table_entry_t *device_driver)
{
	/* fd is fixed for standard input output devices (0, 1, 2)
	 * other number will subject to change depending on the available slot
	 */

	int which_slot = 0;

	if (fd < 0 || fd > NR_DEVICE_DRIVERS - 1) {
		errno = EBADF;
		return -1;
	}

	if ((STDOUT_FILENO == fd) ||
			(STDERR_FILENO == fd) ||
				(STDIN_FILENO == fd)) {
		device_table_list[fd] = device_driver;
		return 0;
	}

	/* search for empty driver slot */
	do {
		if (device_table_list[which_slot] == NULL) break;
		which_slot++;
	} while(which_slot < NR_DEVICE_DRIVERS);

	/* no more slot, notify with error number */
	if (which_slot == NR_DEVICE_DRIVERS) {
		errno = ENOMEM;
		return -1;
	}

	device_table_list[which_slot] = device_driver;

	if (name) device_table_list[which_slot]->name = name;

	return which_slot;
}

int unregister_chrdev(int fd, char *name)
{
	int which_dev = 0;

	if (fd < 0 || fd > NR_DEVICE_DRIVERS - 1) {
		errno = EBADF;
		return -1;
	}

	/* it is not allowed to unregister std devices */
	if ((STDOUT_FILENO == fd) ||
			(STDERR_FILENO == fd) ||
				(STDIN_FILENO == fd)) {
		errno = EACCES;
		return -1;
	}

	/* search for "file" in device_table_list[].name */
	do {
		if (strcmp(device_table_list[which_dev]->name, name) == 0) {
			device_table_list[which_dev]->name = NULL;
			device_table_list[which_dev] = NULL;
			break;
		}
		which_dev++;
	} while(which_dev < NR_DEVICE_DRIVERS);

	/* no device name found, notify with error number */
	if (which_dev == NR_DEVICE_DRIVERS) {
		errno = ENODEV;
		return -1;
	}

	return 0;
}

#define ANY_NUMBER (STDERR_FILENO + 1)

void stage1(void)
{
	/* configure cpu clock */
	clock_device_init();

	/* register default systick's irq handler */
	hook_irq_handler(&systick_irq_hook, SYSTICK_IRQn, NULL);

	/* configure systick */
	systick_device_config(CLOCKS_PER_SEC);

	/* start systick */
	enable_irq(SYSTICK_IRQn, 0);
}

void stage2(void)
{
	int i;

	/* registering standard input output device drivers */
	register_chrdev(STDIN_FILENO, "/dev/stdin", &com1_driver);
	register_chrdev(STDOUT_FILENO, "/dev/stdout", &com1_driver);
	register_chrdev(STDERR_FILENO, "/dev/stderr", &com1_driver);

	/* printf() and scanf() are not useable until stdin stdout stderr device
	 * drivers are loaded and opened.
	 */
	device_table_list[0]->init(0);
	device_table_list[0]->is_open = 1;
	device_table_list[1]->init(1);
	device_table_list[1]->is_open = 1;
	device_table_list[2]->init(2);
	device_table_list[2]->is_open = 1;

	/* first printf() message should appear immediately */
	printf("\n\n");
	printf("*** NodinoSYS v0.1 ***\n");
	printf("Simplified BSD License\n\n");

	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "debug.h:DEBUG_SYSTEM=1\n");
	DEBUGMSG_DEVICE(DEBUGMSG_NORMAL, debug_count, "debug.h:DEBUG_DEVICE=1\n");
	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "compiler.h:systick=%iclk/sec\n", CLOCKS_PER_SEC);
	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "\nstage2 init...\n");

	/* late showing message of stdin stdout and stderr configuration */
	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "sys:stdin -> %s\n", device_table_list[0]->name);
	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "sys:stdout -> %s\n", device_table_list[1]->name);
	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "sys:stderr -> %s\n", device_table_list[2]->name);

	/* turn off buffering, so I/O occurs immediately */
	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "sys:nobuff for stdin/out/err\n");

	setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    /* registering peripheral device drivers and initialization */
	led_register();
	servo_register();
	bldc_register();
#ifdef CONFIG_BOARD_HAVE_AUDIO
	register_chrdev(ANY_NUMBER, "/dev/audio1", &audio1_driver);
#endif
#ifdef CONFIG_BOARD_HAVE_DISK
	register_chrdev(ANY_NUMBER, "/dev/sda1", &disk1_driver);
#endif
	null_register();

	for (i = 3; device_table_list[i]; i++) {
		DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "sys:dev_init:%s\n",
				device_table_list[i]->device_type);

		device_table_list[i]->init(i);
	}

	/* setup callback functions prior to rtc init */
	//rtc_device_setcallback(setup_system_time);

	/* check system time and set it if necessary */
	//rtc_device_init();

	/* other cpu device init */
	//cpu_device_init();
}

void stage3(void)
{
	/* hook and enable COM1 interrupt */
	hook_irq_handler(&com1_irq_hook, COM1_IRQn, NULL);
	enable_irq(COM1_IRQn, COM_TXE_IRQ);
	enable_irq(COM1_IRQn, COM_RXNE_IRQ);
}

void system_welcome(void)
{
	printf("\nWelcome to Aisenke's\n");
	printf("  _   _          _ _\n");
	printf(" | \\ | | ___  __| |_|___   ___\n");
	printf(" |  \\| |/ _ \\/ _' | |  _ \\/ _ \\\n");
	printf(" | |\\  | |_|  |_| | | | |  |_| |\n");
	printf(" |_| \\_|\\___/\\__._|_|_| |_\\___/\n");
	printf("  ____       _          _   _\n");
	printf(" |  _ \\ ___ | |__  ___ | |_|_| ___ ___\n");
	printf(" | |_| / _ \\| '_ \\/ _ \\| __| |/ __/ __|\n");
	printf(" |  _ < |_| | |_|  |_| | |_| | |__\\__ \\\n");
	printf(" |_| \\_\\___/|_.__/\\___/ \\__|_|\\___|___/\n");
	printf("\n");
	printf("For further information check:\n");
	printf("http://www.aisenke.com/nodinorobotics\n\n");
}

void system_init(void)
{
	/* minimal preparation:
	 * - cpu clock init
	 * - systick init
	 */
	stage1();

	/* system console launch and device drivers init */
	stage2();

	/* system interrupt hooks init */
	stage3();

	/* testing Sleep() */
	DEBUGFN_SYSTEM(Sleep(1000)); //FIXME provide a mechanism to check the time

	/* done with system init */
	DEBUGMSG_SYSTEM(DEBUGMSG_NORMAL, debug_count, "system init done!\n\n");

	/* welcome message from the system */
	system_welcome();
}

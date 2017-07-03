/*
 * system.h
 *
 * Created on: Jun 17, 2012
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

#ifndef SYSTEM_H_
#define SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#undef errno
extern int errno;

#include "compiler.h"
#include "board.h"

extern volatile time_t system_tick;
extern char *__env[];

#define CONFIG_SYSTEM_HAVE_CAT
#define CONFIG_SYSTEM_HAVE_ECHO
#define CONFIG_SYSTEM_HAVE_SLEEP

#ifdef CONFIG_BOARD_HAVE_DISK
#define CONFIG_SYSTEM_HAVE_CD
#define CONFIG_SYSTEM_HAVE_ENV
#define CONFIG_SYSTEM_HAVE_LS
#define CONFIG_SYSTEM_HAVE_PWD
#endif

#ifdef CONFIG_BOARD_HAVE_CONSOLE
#define CONFIG_SYSTEM_HAVE_SCROTATE
#endif

#define NR_DEVICE_DRIVERS		256
#define NOT_ASSIGNED_YET		3

#define SYSDEV_NAME_LEN			15
#define SYSDEV_TYPE_LEN			10

#define KEY_ETX					3		/* Ctrl-C escape sequence */

/*extern void dmps_sleep(void);
extern void dmps_wakeup(void);
extern void led_device_blink(void);
extern uint32_t rtc_device_gettime(void);*/

#ifdef __caddr_t_required_
typedef char * caddr_t;
#endif

typedef struct {
	char *name;
	char *device_type;
	int device_number;
	int is_open;
	int (*open)(char *name, int flags, int mode);
	int (*close)(int fd);
	int (*write)(int fd, char *ptr, size_t len);
	int (*read)(int fd, char *ptr, size_t len);
	int (*init)(int fd);
	int (*ioctl)(int fd, int cmd, void *ptr);
} device_table_entry_t;

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

#define __MY_PID	1 /* no process, only main() is supported */

#define tell(x)				_tell(x)
#define kill(x,y)			_kill(x,y)
#define settimeofday(x,y)	_settimeofday(x,y)

extern device_table_entry_t null_driver;
extern device_table_entry_t audio1_driver;
/*extern device_table_entry_t console1_driver;*/
extern device_table_entry_t com1_driver;
/*extern device_table_entry_t com2_driver;
extern device_table_entry_t eth1_driver;*/
extern device_table_entry_t disk1_driver;
/*extern device_table_entry_t led_driver[];
extern device_table_entry_t led2_driver;
extern device_table_entry_t led3_driver;
extern device_table_entry_t led4_driver;
extern device_table_entry_t servo1_driver;
extern device_table_entry_t servo2_driver;
extern device_table_entry_t servo3_driver;
extern device_table_entry_t servo4_driver;*/

extern device_table_entry_t *device_table_list[];

int register_chrdev(int fd, char *name, device_table_entry_t *device_driver);
int unregister_chrdev(int fd, char *name);

int ioctl(int fd, unsigned long cmd_request, void *ptr);
int keypressed(void);
int _write(int fd, char *ptr, size_t len);
int _settimeofday(const struct timeval * tp, const struct timezone * tzvp);
int register_chrdev(int fd, char *name, device_table_entry_t *device_driver);
int stdev_close(int fd);
int stdev_open(char *name, int flags, int mode);
void system_init(void);
int unregister_chrdev(int fd, char *name);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H_ */

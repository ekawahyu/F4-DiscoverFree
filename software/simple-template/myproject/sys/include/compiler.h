/*
 * compiler.h
 *
 * Created on: Apr 22, 2011
 *     Author: Ekawahyu Susilo
 *
 * Copyright (c) 2011-2012, Chongqing Aisenke Electronic Technology Co., Ltd.
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

#ifndef COMPILER_H_
#define COMPILER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <fcntl.h>

#ifdef __GNUC__
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/signal.h>
#ifndef Sleep
#define Sleep(x)			usleep((x*1000))
#endif
#define __align4			__attribute__((aligned (4)))
#define __weak				__attribute__((weak))
#define __always_inline		__attribute__((always_inline))
#endif

#ifdef __ICCARM__
#include <yfuns.h>
#define __caddr_t_required_
#define __useconds_t_required_
#define __tms_struct_required_
#define __stat_struct_required_
#define STDIN_FILENO        _LLIO_STDIN
#define STDOUT_FILENO       _LLIO_STDOUT
#define STDERR_FILENO       _LLIO_STDERR
#define __align4			/* TBD */
#define __weak				/* TBD */
#define _exit               __exit
#ifndef Sleep
#define Sleep(x)			usleep((x*1000))
#endif
#include "sleep.h"
#endif

#ifdef _MSC_VER
#include <windows.h>
#include <io.h>
#define __align4			/* no implementation */
#define __weak				/* no implementation */
#define __always_inline
#endif

#ifndef __IO
#define __IO		volatile
#endif

/* redefine CLOCKS_PER_SEC */
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC		100000

#ifdef __cplusplus
}
#endif

#endif /* COMPILER_H_ */

/*
 * nsh.h
 *
 * Created on: Jul 11, 2012
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

#ifndef NSH_H_
#define NSH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

/* uncomment line below if building for native unix system */
/* #define __NATIVE_SYSTEM */

/* define additional user executable */
#ifndef NSH_USER_PROVIDE_EXECS
#define NSH_USER_EXECS
#warning "NSH_USER_PROVIDE_EXECS and NSH_USER_EXECS are not defined! user executable are not included!"
#else
#include "userexec.h"
#endif

//#define NSH_PROMPT				"\033[37m\033[44mnsh%s>\033[0m "
#define NSH_PROMPT				"nsh%s> "
#define NSH_PROMPT_LEN			6
#define NSH_CMDLINE_LEN			256
#define NSH_CMDHISTORY_LEN		256
#define NSH_MAX_ARGV			20

#define KEY_ETX			3
#define KEY_BS			8
#define KEY_LF			10
#define KEY_CR			13
#define KEY_SO			14
#define KEY_DLE			16
#define KEY_ESC			27
#define KEY_DEL			127

#define NSH_NO_ESC_SEQ				0
#define NSH_ESC_SEQ_DETECTED		1
#define NSH_ESC_BRACKET_DETECTED	2
#define NSH_CAPTURE_ESC_SEQ			3
#define NSH_PARSING_ESC_SEQ			4
#define NSH_PROCESS_ESC_SEQ			5

#define NSH_HISTORY_UP		0
#define NSH_HISTORY_DOWN	1

#define NSH_INPUT_TEXT		0
#define NSH_INPUT_PASSWORD	1
#define NSH_INPUT_SILENT	2

typedef struct {
	char * cmd;
	char * desc;
	int (*appcall)(int argc, char *argv[]);
	int (*helpcall)(int argc, char *argv[]);
} nsh_exec_t;

typedef struct {
	char * cmd;
	char * desc;
	int (*appcall)(int argc, char *argv[]);
	int (*helpcall)(int argc, char *argv[]);
} nsh_command_t;

typedef struct {
	char cmdhist_buffer[NSH_CMDHISTORY_LEN];
	char * pcmdhist;
	char * pcmdhist_search;
	char * pcmdhist_bottom;
	char * pcmdhist_top;
	char cmdline[NSH_CMDLINE_LEN];
	char * pcmdline;
	int argc;
	char * argv[NSH_MAX_ARGV];
	int input_type;
	int state;
} nsh_instance_t;

int nsh_app(int argc, char * argv[]);
static inline void nsh_backspace(nsh_instance_t * inst);
static inline int nsh_clear(int argc, char * argv[]);
void nsh_edit_line(nsh_instance_t * inst, int key);
static inline int nsh_exit(int argc, char * argv[]);
static inline int nsh_get_char(void);
void nsh_get_history(nsh_instance_t * inst, int dir);
int nsh_help(int argc, char * argv[]);
static inline void nsh_init(nsh_instance_t * inst);
int nsh_list(int argc, char * argv[]);
void nsh_process_line(nsh_instance_t * inst);
void nsh_prompt (nsh_instance_t * inst, char *path);
void nsh_save_history(nsh_instance_t * inst);
int nsh_service(nsh_instance_t * inst);

#ifdef __cplusplus
}
#endif

#endif /* NSH_H_ */

/*
 * nsh.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <setjmp.h>
#ifdef __NATIVE_SYSTEM
#include <termios.h>
#endif
#include "system.h"
#include "nsh.h"

extern int klbasic_app(int argc, char * argv[]);
extern int nplay_app(int argc, char * argv[]);
extern int nplay_help(int argc, char * argv[]);
extern int picoc_app(int argc, char * argv[]);
extern int tinybasic_app(int argc, char * argv[]);
extern int bldc_app(int argc, char * argv[]);

extern int cat_app(int argc, char * argv[]);
extern int cd_app(int argc, char * argv[]);
extern int echo_app(int argc, char * argv[]);
extern int echo_help(int argc, char * argv[]);
extern int env_app(int argc, char * argv[]);
extern int ls_app(int argc, char * argv[]);
extern int ls_help(int argc, char * argv[]);
extern int pwd_app(int argc, char * argv[]);
extern int scrotate_app(int argc, char * argv[]);
extern int sleep_app(int argc, char * argv[]);
extern int sleep_help(int argc, char * argv[]);
extern int bldc_help(int argc, char * argv[]);

nsh_exec_t nsh_exec_list[] = {
	{"help", "- show this help", nsh_help, NULL},
	{"list", "- show commands", nsh_list, NULL},
#ifdef CONFIG_APPS_INSTALL_KLBASIC
	{"klbasic", "- KLBasic interpreter", klbasic_app, NULL},
#endif
#ifdef CONFIG_APPS_INSTALL_BLDC
	{"bldc", "- brushless control", bldc_app, bldc_help},
#endif
#ifdef CONFIG_APPS_INSTALL_NPLAY
	{"nplay", "- multimedia player", nplay_app, nplay_help},
#endif
#ifdef CONFIG_APPS_INSTALL_NSH
	{"nsh", "- nanoshell", nsh_app, NULL},
#endif
#ifdef CONFIG_APPS_INSTALL_PICOC
	{"picoc", "- Pico C interpreter", picoc_app, NULL},
#endif
#ifdef CONFIG_APPS_INSTALL_TINYBASIC
	{"tinybasic", "- TinyBasic interpreter", tinybasic_app, NULL},
#endif
	NSH_USER_EXECS
	{NULL, NULL, NULL}
};

nsh_command_t nsh_command_list[] = {
#ifdef CONFIG_SYSTEM_HAVE_CAT
	{"cat", "- concatenate and print files", cat_app, NULL},
#endif
#ifdef CONFIG_SYSTEM_HAVE_CD
	{"cd", "- change directory", cd_app, NULL},
#endif
	{"clear", "- clear screen", nsh_clear, NULL},
#ifdef CONFIG_SYSTEM_HAVE_ECHO
	{"echo", "- write arguments to stdout", echo_app, echo_help},
#endif
#ifdef CONFIG_SYSTEM_HAVE_ENV
	{"env", "- environment vars", env_app, NULL},
#endif
	{"exit", "- exit shell", nsh_exit, NULL},
#ifdef CONFIG_SYSTEM_HAVE_LS
	{"ls", "- dir listing", ls_app, ls_help},
#endif
#ifdef CONFIG_SYSTEM_HAVE_PWD
	{"pwd", "- working dir", pwd_app, NULL},
#endif
#ifdef CONFIG_SYSTEM_HAVE_SCROTATE
	{"scrotate", "- screen orientation", scrotate_app, NULL},
#endif
#ifdef CONFIG_SYSTEM_HAVE_SLEEP
	{"sleep", "- sleep seconds", sleep_app, sleep_help},
#endif
	{NULL, NULL, NULL}
};

void nsh_sigint_handler (int signum);

int nsh_app(int argc, char * argv[])
{
	nsh_instance_t inst;

	nsh_init(&inst);

	signal(SIGINT, (void*)nsh_sigint_handler);

	while(nsh_service(&inst) != KEY_ETX);

	printf("\ngoodbye!\n");

	return 0;
}

static inline void nsh_backspace(nsh_instance_t * inst)
{
	if (inst->pcmdline != inst->cmdline) {
		inst->pcmdline--;
		*inst->pcmdline = '\0';
		printf("\033[D \033[D");
	}
}

static inline int nsh_clear(int argc, char * argv[])
{
	printf("\033[2J\033[H");

	return 0;
}

void nsh_edit_line(nsh_instance_t * inst, int key)
{
	if (inst->pcmdline != &inst->cmdline[NSH_CMDLINE_LEN - 1]) {
		*inst->pcmdline++ = key;
		printf("%c", key);
	}
}

static inline int nsh_exit(int argc, char * argv[])
{
	printf("press Ctrl-C to exit\n");

	return 0;
}

#ifdef __NATIVE_SYSTEM
static inline int nsh_get_char(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );

	ch = getchar();

	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

	return ch;
}
#else
static inline int nsh_get_char(void)
{
	return getchar();
}
#endif

void nsh_get_history(nsh_instance_t * inst, int dir)
{
	char * search_hit;

	if (dir == NSH_HISTORY_UP) {

		inst->pcmdhist_search--;
		if (inst->pcmdhist_search < inst->pcmdhist_bottom)
			inst->pcmdhist_search = inst->pcmdhist_top;

		while ((*inst->pcmdhist_search == '\0') &&
				(inst->pcmdhist_search != inst->pcmdhist)) {
			inst->pcmdhist_search--;
			if (inst->pcmdhist_search < inst->pcmdhist_bottom)
				inst->pcmdhist_search = inst->pcmdhist_top;
		}

		while (*inst->pcmdhist_search && (inst->pcmdhist_search != inst->pcmdhist)) {
			inst->pcmdhist_search--;
			if (inst->pcmdhist_search < inst->pcmdhist_bottom)
				inst->pcmdhist_search = inst->pcmdhist_top;
		}

		inst->pcmdhist_search++;
		if (inst->pcmdhist_search > inst->pcmdhist_top)
			inst->pcmdhist_search = inst->pcmdhist_bottom;
	}

	if (dir == NSH_HISTORY_DOWN) {

		while (*inst->pcmdhist_search && (inst->pcmdhist_search != inst->pcmdhist)) {
			inst->pcmdhist_search++;
			if (inst->pcmdhist_search > inst->pcmdhist_top)
				inst->pcmdhist_search = inst->pcmdhist_bottom;
		}

		while ((*inst->pcmdhist_search == '\0') && (inst->pcmdhist_search != inst->pcmdhist)) {
			inst->pcmdhist_search++;
			if (inst->pcmdhist_search > inst->pcmdhist_top)
				inst->pcmdhist_search = inst->pcmdhist_bottom;
		}
	}

	printf("\033[%iD", NSH_CMDLINE_LEN + NSH_PROMPT_LEN);
	nsh_prompt(inst, NULL);
	printf("\033[K");

	search_hit = inst->pcmdhist_search;

	while (*search_hit) {
		nsh_edit_line(inst, *search_hit++);
		if (search_hit > inst->pcmdhist_top)
			search_hit = inst->pcmdhist_bottom;
	}
}

int nsh_help(int argc, char * argv[])
{
	nsh_exec_t * pnsh_exec_list = nsh_exec_list;
	nsh_command_t * pnsh_command_list = nsh_command_list;

	if (argc > 1) {

		while (pnsh_exec_list->cmd) {
			if (strcmp(pnsh_exec_list->cmd, argv[1]) == 0) {
				if (pnsh_exec_list->helpcall)
					pnsh_exec_list->helpcall(argc, argv);
				else
					fprintf(stderr, "nsh: \"%s\" help not available\n",
							pnsh_exec_list->cmd);
				break;
			}
			pnsh_exec_list++;
		}

		while (pnsh_command_list->cmd) {
			if (strcmp(pnsh_command_list->cmd, argv[1]) == 0) {
				if (pnsh_command_list->helpcall)
					pnsh_command_list->helpcall(argc, argv);
				else
					fprintf(stderr, "nsh: \"%s\" help not available\n",
							pnsh_command_list->cmd);
				break;
			}
			pnsh_command_list++;
		}

		if (pnsh_exec_list->cmd == NULL && pnsh_command_list->cmd == NULL)
			fprintf(stderr, "nsh: \"%s\" not found\n", argv[1]);
	}
	else {

		printf("nanoshell v0.1\n");
		printf("usage: help [COMMAND]\n");

		while (pnsh_exec_list->cmd) {
			printf(" * %s %s\n", pnsh_exec_list->cmd, pnsh_exec_list->desc);
			pnsh_exec_list++;
		}
	}

	return 0;
}

static inline void nsh_init(nsh_instance_t * inst)
{
	/*inst->pcmdhist = inst->cmdhist_buffer;
	inst->pcmdhist_search = inst->cmdhist_buffer;
	inst->pcmdhist_bottom = inst->cmdhist_buffer;
	inst->pcmdhist_top = &inst->cmdhist_buffer[NSH_CMDHISTORY_LEN - 1];
	inst->pcmdline = inst->cmdline;*/

	/* reset command history to empty */
	memset(inst->cmdhist_buffer, 0, sizeof(inst->cmdhist_buffer[0]) * NSH_CMDHISTORY_LEN);
	inst->pcmdhist = inst->cmdhist_buffer;
	inst->pcmdhist_search = inst->cmdhist_buffer;
	inst->pcmdhist_bottom = inst->cmdhist_buffer;
	inst->pcmdhist_top = &inst->cmdhist_buffer[NSH_CMDHISTORY_LEN - 1];

	inst->state = NSH_NO_ESC_SEQ;
	inst->input_type = NSH_INPUT_TEXT;

	nsh_prompt(inst, NULL);
}

int nsh_list(int argc, char * argv[])
{
	nsh_command_t * pnsh_command_list = nsh_command_list;

	while (pnsh_command_list->cmd) {
		printf("%s\n", pnsh_command_list->cmd);
		pnsh_command_list++;
	}

	return 0;
}

jmp_buf __nsh_exec_env;
volatile int __nsh_exec_in_progress;

void nsh_process_line(nsh_instance_t * inst)
{
	nsh_exec_t * pnsh_exec_list = nsh_exec_list;
	nsh_command_t * pnsh_command_list = nsh_command_list;
	char * token;
	int i;

	inst->argc = 0;

	printf("\n");
	nsh_save_history(inst);

	token = strtok(inst->cmdline, " \"");
	if (token) {
		inst->argc++;
		inst->argv[inst->argc-1] = calloc(strlen(token)+1, sizeof(inst->argv));
		assert(inst->argv[inst->argc-1] != NULL);
		memcpy(inst->argv[inst->argc-1], token, strlen(token));
	}
	do {
		token = strtok(NULL, " \"");
		if (token) {
			if (inst->argc < NSH_MAX_ARGV) {
				inst->argc++;
				inst->argv[inst->argc-1] = calloc(strlen(token)+1, sizeof(inst->argv));
				assert(inst->argv[inst->argc-1] != NULL);
				memcpy(inst->argv[inst->argc-1], token, strlen(token));
			}
		}
		else
			inst->argv[inst->argc] = NULL;

	} while (token);

	if (inst->argc) {

		while (pnsh_exec_list->cmd) {
			if (strcmp(pnsh_exec_list->cmd, inst->argv[0]) == 0)
				if (pnsh_exec_list->appcall) {
					if (setjmp(__nsh_exec_env) == 0) {
						__nsh_exec_in_progress = 1;
						pnsh_exec_list->appcall(inst->argc, inst->argv);
					}
					__nsh_exec_in_progress = 0;
					break;
				}
			pnsh_exec_list++;
		}

		while (pnsh_command_list->cmd) {
			if (strcmp(pnsh_command_list->cmd, inst->argv[0]) == 0)
				if (pnsh_command_list->appcall) {
					if (setjmp(__nsh_exec_env) == 0) {
						__nsh_exec_in_progress = 1;
						pnsh_command_list->appcall(inst->argc, inst->argv);
					}
					__nsh_exec_in_progress = 0;
					break;
				}
			pnsh_command_list++;
		}

		if (pnsh_exec_list->cmd == NULL && pnsh_command_list->cmd == NULL)
			fprintf(stderr, "nsh: \"%s\" not found\n", inst->argv[0]);
	}

	for (i = 0; i < inst->argc; i++) free(inst->argv[i]);

	nsh_prompt(inst, NULL);
}

void nsh_prompt (nsh_instance_t * inst, char * path)
{
	/* reset command line to empty */
	memset(inst->cmdline, 0, sizeof(inst->cmdline[0]) * NSH_CMDLINE_LEN);
	inst->pcmdline = inst->cmdline;

	if (path == NULL)
		printf(NSH_PROMPT, "/");
	else
		printf(NSH_PROMPT, path);
}

void nsh_save_history(nsh_instance_t * inst)
{
	inst->pcmdline = inst->cmdline;

	if (*inst->pcmdline == '\0') return;

	while (*inst->pcmdline) {
		*inst->pcmdhist++ = *inst->pcmdline++;
		if (inst->pcmdhist > inst->pcmdhist_top) inst->pcmdhist = inst->pcmdhist_bottom;
	}

	*inst->pcmdhist++ = '\0';
	if (inst->pcmdhist > inst->pcmdhist_top) inst->pcmdhist = inst->pcmdhist_bottom;

	inst->pcmdhist_search = inst->pcmdhist;
}

int nsh_service(nsh_instance_t * inst)
{
	int key;

	key = nsh_get_char();

	switch (inst->state) {

	case NSH_NO_ESC_SEQ:
		switch (key) {
		case KEY_ETX:
			//return KEY_ETX;
			break;
		case KEY_LF:
			//nsh_process_line(inst);
			break;
		case KEY_CR:
			nsh_process_line(inst);
			break;
		case KEY_ESC:
			inst->state = NSH_ESC_SEQ_DETECTED;
			break;
		case KEY_BS:
		case KEY_DEL:
			nsh_backspace(inst);
			break;
		default:
			nsh_edit_line(inst, key);
			break;
		}
		break; /* NSH_NO_ESC_SEQ */

	case NSH_ESC_SEQ_DETECTED:
		if (key == '[') inst->state = NSH_ESC_BRACKET_DETECTED;
		else inst->state = NSH_NO_ESC_SEQ;
		break; /* NSH_ESC_SEQ_DETECTED */

	case NSH_ESC_BRACKET_DETECTED:
		if (key == '[') inst->state = NSH_ESC_BRACKET_DETECTED;
		else {
			if (key == 'A') nsh_get_history(inst, NSH_HISTORY_UP);
			if (key == 'B') nsh_get_history(inst, NSH_HISTORY_DOWN);
			if (key == 'C'){}; /* not applicable at the moment */
			if (key == 'D') nsh_backspace(inst);
			inst->state = NSH_NO_ESC_SEQ;
		}
		break;

	} /* state */

	return 0;
}

void nsh_sigint_handler(int signum)
{
	//printf("nsh sigint handler\n");
	if (__nsh_exec_in_progress) longjmp(__nsh_exec_env, 1);
	signal(SIGINT, (void*)nsh_sigint_handler);
}

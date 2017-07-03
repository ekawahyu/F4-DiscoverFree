/*
 * bldc.c
 *
 * Created on: Jan 12, 2013
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "system.h"

int bldc_app(int argc, char *argv[])
{
	int result = -1;
	int port_number;

	if (*++argv && !strcmp(*argv, "-p")) {
		++argv;
		port_number = atol(*argv);
	}
	else {
		printf("port number required\n");
		return result;
	}

	while (*++argv != NULL) {

		if (*argv && !strcmp(*argv, "direction")) {
			printf("/dev/bldc%i direction = %i\n", port_number, bldc_get_direction(port_number));
		}
		else if (*argv && !strcmp(*argv, "cw")) {
			bldc_cw(port_number);
		}
		else if (*argv && !strcmp(*argv, "ccw")) {
			bldc_ccw(port_number);
		}
		else if (*argv && !strcmp(*argv, "stop")) {
			bldc_stop(port_number);
		}
		else if (*argv && !strcmp(*argv, "step_cw")) {
			bldc_step_cw(port_number, atol(*++argv));
		}
		else if (*argv && !strcmp(*argv, "step_ccw")) {
			bldc_step_ccw(port_number, atol(*++argv));
		}
		else if (*argv && !strcmp(*argv, "position")) {
			printf("/dev/bldc%i position = %li\n", port_number, bldc_get_position(port_number));
		}
		else if (*argv && !strcmp(*argv, "position_reset")) {
			bldc_reset_position(port_number);
		}
		else if (*argv && !strcmp(*argv, "anchor")) {
			printf("/dev/bldc%i anchor = %li\n", port_number, bldc_get_anchor(port_number));
		}
		else if (*argv && !strcmp(*argv, "anchor_set")) {
			bldc_set_anchor(port_number, atol(*++argv));
		}
		else if (*argv && !strcmp(*argv, "button")) {
			printf("/dev/bldc%i button mode = %i\n", port_number, bldc_get_button_mode(port_number));
		}
		else if (*argv && !strcmp(*argv, "button_off")) {
			bldc_button_mode_off(port_number);
		}
		else if (*argv && !strcmp(*argv, "button_on")) {
			bldc_button_mode_on(port_number);
		}
		else if (*argv && !strcmp(*argv, "bemf")) {
			printf("/dev/bldc%i bemf mode = %i\n", port_number, bldc_get_bemf_mode(port_number));
		}
		else if (*argv && !strcmp(*argv, "bemf_off")) {
			bldc_bemf_mode_off(port_number);
		}
		else if (*argv && !strcmp(*argv, "bemf_on")) {
			bldc_bemf_mode_on(port_number);
		}
	}

	return 0;
}

int bldc_help(int argc, char **argv)
{
	printf("NodinoSYS BLDC v0.1\nusage: bldc -p [port] [commands]\nopts:\n");
	printf("  -p [port]\t\tport number\ncommands:\n");
	printf("  anchor\t\treturn anchor location\n");
	printf("  anchor_set [pos]\tset new anchor location\n");
	printf("  bemf\t\t\treturn bemf mode\n");
	printf("  bemf_on\t\tenable bemf mode\n");
	printf("  bemf_off\t\tdisable bemf mode\n");
	printf("  button\t\treturn button mode\n");
	printf("  button_on\t\tbuttons control\n");
	printf("  button_off\t\tcommand line control\n");
	printf("  position\t\treturn rotor position\n");
	printf("  position_reset\treset position to zero\n");
	printf("  step_cw [n]\t\tstep CW n steps\n");
	printf("  step_ccw [n]\t\tstep CCW n steps\n");
	printf("  direction\t\tget current motor movement\n");
	printf("  cw\t\t\tforce CW (use with caution)\n");
	printf("  ccw\t\t\tforce CCW (use with caution)\n");
	printf("  stop\t\t\tforce to stop\n");

	return 0;
}

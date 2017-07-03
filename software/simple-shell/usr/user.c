/*
 * user.c
 *
 * Created on: Dec 12, 2012
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
#include "config.h"
#include "system.h"
#include "adc.h"
#include "button.h"
#include "bldc.h"
#include "servo.h"

#include "osp.h"

extern int nsh_app(int argc, char * argv[]);

void setup(void)
{

}

void task0(void);
void task1(void);
void task2(void);
void task3(void);


void task0(void)
{
	int i = 0;

	osSemInit( 0, 0);
    osSemInit( 1, 0);
    osSemInit( 2, 0);
	osSemInit( 3, 0);

    osTaskCreate( task1 );
    osTaskCreate( task2 );


     while(1) {
        printf( "Task 0 P0 loop(%i)\n", i++ );
        osSemP(0);
        osTaskCreate( task3 );

    }
    osTaskExit();
}

void task1(void)
{
	int i = 0;

	while(1) {
        printf( "Task 1 P1 loop(%i)\n", i++ );
        osSemP(1);

        printf( "Task 1 V2\n" );
        osSemV(2);

    }
    osTaskExit();
}

void task2(void)
{
	int i = 0;

	while(1) {
        printf( "Task 2 P2 loop(%i)\n", i++ );
        osSemP(2);
    }
    osTaskExit();
}

void task3(void)
{
	int i = 0;

    printf( "Task 3 created and exit (%i)\n", i++ );

    osTaskExit();
}

void taskKeyboard(void)
{
	char c;

    while(1){
        osTaskPreempt();
        if( keypressed() ){
            c = getchar();
            switch (c){
			case 'b':
				osSemV(0);
				break;
			case 'n':
				osSemV(1);
				break;
			case 'm':
				osSemV(2);
				osSemV(2);
				osSemV(2);
				osSemV(2);
				break;
			case '0':
				//LATA &=(~0b11100000000);
				//LD1_O = 0;
				//IO1_O = 0;
				//SD_O = 0;
				break;
			case '1':
				//LD1_O = 1;
				break;
			case '2':
				//IO1_O = 1;
				break;
			case '3':
				//SD_O = 1;
				break;
			default:
				break;
            }
        }

    }
}

void dhry_main();

void loop(void)
{
	dhry_main();

	while(1);

    osInit();
    osTaskCreate(taskKeyboard);
    osTaskCreate(task0);
    osBegin();

#ifdef CONFIG_APPS_INSTALL_NSH
	nsh_app(0, NULL);
#endif
}

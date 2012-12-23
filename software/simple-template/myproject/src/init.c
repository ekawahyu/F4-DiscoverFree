/*
 * init.c
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

#include "init.h"

FRESULT res;
FILINFO fno;
FIL fil;
DIR dir;
FATFS fs32;

void accelerometer_config(void)
{
	/* Configure ADC DMA for accelerometer reading */
	printf("configuring accelerometer...\n");
	ADC1_CH8_DMA_Config();
	ADC2_CH9_DMA_Config();
	ADC3_CH3_DMA_Config();

	/* Start ADC conversion */
	ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC2);
	ADC_SoftwareStartConv(ADC3);
}

void audio_config(void)
{
	/* Configure audio codec */
	printf("configuring audio driver...\n");
	vs1053b_config();
	vs1053b_soft_reset();
}

void system_init(void)
{
	/* Configure system tick */
	systick_device_config(CLOCKS_PER_SEC);

	/* Configure serial port as terminal */
	usart_config();

	/* No buffering, serial input/output occurs immediately */
	setvbuf(stdin,  NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf("\nF4-DiscoverFree system init...\n");

	/* Configure microSD card through SDIO */
	printf("configuring file system...\n");
	memset(&fs32, 0, sizeof(FATFS));
	res = f_mount(0, &fs32);

	printf("done!\n\n");

	printf("Welcome to Aisenke's\n");
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
	printf("http://www.aisenke.com/nodinorobotics\n");
	printf("\n");
}

/*
 * main.c
 *
 * Created on: Nov 25, 2012
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

#include "led.h"
#include "button.h"
#include "adc.h"
#include "usart.h"
#include "vs1053b.h"
#include "ff.h"

#define DEMO_LED				0
#define DEMO_OGG_PLAYER			1
#define DEMO_ACCELEROMETER		2

#define NUMBER_OF_SONGS			10

#define DELAY_CONST				0x00FFFFFF

typedef struct {
	char * filename;
} song_t;

int demo_mode = DEMO_ACCELEROMETER;

int song_index = 0;
song_t song[NUMBER_OF_SONGS];
song_t * psong = song;

volatile uint16_t accel_volume_count = 0;
uint16_t accel_volume = 0x2020;

FRESULT res;
FILINFO fno;
FIL fil;
DIR dir;
FATFS fs32;

void led_show_transition(void)
{
	volatile uint32_t delay;

	led_on(LED1);
	led_on(LED2);
	led_on(LED3);
	led_on(LED4);

	delay = DELAY_CONST;
	while(delay--);

	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	led_off(LED4);

	delay = DELAY_CONST;
	while(delay--);
}

void demo_led(void)
{
	volatile uint32_t delay;

	led_on(LED1);

	delay = DELAY_CONST >> 1;
	while(delay--);

	led_on(LED2);

	delay = DELAY_CONST >> 1;
	while(delay--);

	led_on(LED3);

	delay = DELAY_CONST >> 1;
	while(delay--);

	led_on(LED4);

	delay = DELAY_CONST >> 1;
	while(delay--);

	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	led_off(LED4);

	delay = DELAY_CONST >> 1;
	while(delay--);

	if (button_getstate(BUTTON_S2) == 1) {

		printf("Entering Accelerometer Demo...\n");
		led_show_transition();
		while(button_getstate(BUTTON_S2) == 1);
		demo_mode = DEMO_ACCELEROMETER;
	}
}

void demo_accelerometer(void)
{
	if (ADC3ConvertedValue < 1700) {

		led_on(LED1);
		led_on(LED2);
		led_on(LED3);
		led_on(LED4);
	}
	else {

		if (ADC1ConvertedValue > 2000) led_on(LED1);
		else led_off(LED1);

		if (ADC1ConvertedValue < 1700) led_on(LED3);
		else led_off(LED3);

		if (ADC2ConvertedValue > 2000) led_on(LED4);
		else led_off(LED4);

		if (ADC2ConvertedValue < 1700) led_on(LED2);
		else led_off(LED2);
	}

	if (button_getstate(BUTTON_S2) == 1) {

		printf("Entering Music Player Demo...\n");
		led_show_transition();
		while(button_getstate(BUTTON_S2) == 1);
		demo_mode = DEMO_OGG_PLAYER;
	}
}

void demo_ogg_player(void)
{
	volatile uint32_t delay;

	BYTE Buffer[32];
	UINT BytesRead;

	vs1053b_soft_reset();
	vs1053b_set_volume(accel_volume);

	led_on(LED1);

	res = f_open(&fil, song[song_index].filename, FA_READ);
	printf("music_player: playing %s\n", song[song_index].filename);

	if (res == FR_OK)
	{
		while(1)
		{
			res = f_read(&fil, Buffer, sizeof(Buffer), &BytesRead);

			if (res != FR_OK)
				break;

			if (ADC1ConvertedValue > 1900) {

				accel_volume_count++;

				if (accel_volume_count > 0x001F) {
					accel_volume_count = 0;
					accel_volume -= 0x0101;
				}

				led_on(LED1);
			}
			else led_off(LED1);

			if (ADC1ConvertedValue < 1550) {

				accel_volume_count++;

				if (accel_volume_count > 0x001F) {
					accel_volume_count = 0;
					accel_volume += 0x0101;
				}

				led_on(LED3);
			}
			else led_off(LED3);

			if (ADC2ConvertedValue > 2050) {

				song_index--;
				led_on(LED4);
				break;
			}
			else led_off(LED4);

			if (ADC2ConvertedValue < 1550) {

				song_index++;
				led_on(LED2);
				break;
			}
			else led_off(LED2);

			if (button_getstate(BUTTON_S2) == 1) {

				printf("Entering LED Demo...\n");
				led_show_transition();
				while(button_getstate(BUTTON_S2) == 1);
				demo_mode = DEMO_LED;
				break;
			}

			vs1053b_write_data((void*)(Buffer));

			if (BytesRead < sizeof(Buffer)) {

				song_index++;
				break;
			}

			if (accel_volume > 0xFEFE) accel_volume = 0xFEFE;
			if (accel_volume < 0x0101) accel_volume = 0x0101;

			vs1053b_set_volume(accel_volume);
		}

		res = f_close(&fil);

		delay = DELAY_CONST;
		while(delay--);

		if (song_index < 0) song_index = NUMBER_OF_SONGS - 1;
		if (song_index > NUMBER_OF_SONGS -1 ) song_index = 0;
	}
}

int main(void)
{
	BYTE Buffer[128];
	UINT BytesRead;

	/* Configure serial port as terminal */
	usart_config();

	/* No buffering, serial input/output occurs immediately */
	setvbuf(stdin,  NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf("\nF4-DiscoverFree is initializing system...\n");

	/* Configure ADC DMA for accelerometer reading */
	printf("configuring ADCs...\n");
	ADC1_CH8_DMA_Config();
	ADC2_CH9_DMA_Config();
	ADC3_CH3_DMA_Config();

	/* Start ADC conversion */
	ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC2);
	ADC_SoftwareStartConv(ADC3);

	/* Configure LEDs */
	printf("configuring LEDs...\n");
	led_config(LED1);
	led_config(LED2);
	led_config(LED3);
	led_config(LED4);

	/* Configure button S2 */
	printf("configuring button S2...\n");
	button_config(BUTTON_S2);

	/* Configure microSD card through SDIO */
	printf("configuring file system...\n");
	memset(&fs32, 0, sizeof(FATFS));
	res = f_mount(0, &fs32);

	/* Configure audio codec */
	printf("configuring audio driver...\n");
	vs1053b_config();
	vs1053b_soft_reset();
	vs1053b_set_volume(accel_volume);

	/* Loading songs list */
	printf("loading playlist...\n");
	psong++->filename = "ogg/track01.ogg";
	psong++->filename = "ogg/track02.ogg";
	psong++->filename = "ogg/track03.ogg";
	psong++->filename = "ogg/track04.ogg";
	psong++->filename = "ogg/track05.ogg";
	psong++->filename = "ogg/track06.ogg";
	psong++->filename = "ogg/track07.ogg";
	psong++->filename = "ogg/track08.ogg";
	psong++->filename = "ogg/track09.ogg";
	psong++->filename = "ogg/track10.ogg";

	printf("reading welcome message from microSD card...\n\n");
	res = f_open(&fil, "welcome.txt", FA_READ);

	if (res == FR_OK) {

		do {
			res = f_read(&fil, Buffer, sizeof(Buffer)-1, &BytesRead);
			Buffer[BytesRead] = '\0';
			printf("%s", Buffer);
		} while (BytesRead);

		res = f_close(&fil);
	}
	else printf("error: reading microSD failed! (%i)\n", res);

	printf("\n");

	demo_led();

	printf("Press S2 to switch between demos!\n");
	while (1) {
		if (demo_mode == DEMO_LED) {
			demo_led();
		}

		if (demo_mode == DEMO_ACCELEROMETER) {
			demo_accelerometer();
		}

		if (demo_mode == DEMO_OGG_PLAYER) {
			demo_ogg_player();
		}
	}
}

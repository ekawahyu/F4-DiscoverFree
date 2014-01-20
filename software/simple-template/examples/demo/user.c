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

#include "init.h"

#include "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"

#define DEMO_LED				0
#define DEMO_OGG_PLAYER			1
#define DEMO_ACCELEROMETER		2
#define DEMO_SERVO				3
#define DEMO_USB_HID			4

#define NUMBER_OF_SONGS			300

USB_OTG_CORE_HANDLE USB_OTG_dev;

typedef struct {
	char filename[_MAX_LFN];
} song_t;

int demo_mode = DEMO_ACCELEROMETER;

int song_total = 0;
int song_index = 0;
song_t songs[NUMBER_OF_SONGS];
song_t * psong = songs;

volatile uint16_t accel_volume_count = 0;
uint16_t accel_volume = 0x5050;

void led_show_transition(void)
{
	led_on(LED1);
	led_on(LED2);
	led_on(LED3);
	led_on(LED4);

	Sleep(500);

	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	led_off(LED4);

	Sleep(500);
}

void demo_led(void)
{
	led_on(LED1);

	Sleep(500);

	led_on(LED2);

	Sleep(500);

	led_on(LED3);

	Sleep(500);

	led_on(LED4);

	Sleep(500);

	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	led_off(LED4);

	Sleep(500);

	if (button_getstate(BUTTON_S2) == 1) {

		printf("Entering Accelerometer Demo...\n");
		led_show_transition();
		while(button_getstate(BUTTON_S2) == 1);
		demo_mode = DEMO_ACCELEROMETER;
	}
}

void demo_accelerometer(void)
{
	if (accelerometer_read(ACCEL_Z) < 1800) {

		led_on(LED1);
		led_on(LED2);
		led_on(LED3);
		led_on(LED4);
	}
	else {

		if (accelerometer_read(ACCEL_X) > 2200)
			led_on(LED1);
		else
			led_off(LED1);

		if (accelerometer_read(ACCEL_X) < 1800)
			led_on(LED3);
		else
			led_off(LED3);

		if (accelerometer_read(ACCEL_Y) > 2200)
			led_on(LED4);
		else
			led_off(LED4);

		if (accelerometer_read(ACCEL_Y) < 1800)
			led_on(LED2);
		else
			led_off(LED2);
	}

	if (button_getstate(BUTTON_S2) == 1) {

		printf("Entering Music Player Demo...\n");
		led_show_transition();
		while(button_getstate(BUTTON_S2) == 1);
		demo_mode = DEMO_OGG_PLAYER;
	}
}

void demo_music_player(void)
{
	BYTE Buffer[32];
	UINT BytesRead;

	audio_soft_reset();
	audio_set_volume(accel_volume);

	led_on(LED1);

	res = f_open(&fil, songs[song_index].filename, FA_READ);
	printf("music_player: playing %s\n", songs[song_index].filename);

	if (res == FR_OK) {
		while (1) {
			res = f_read(&fil, Buffer, sizeof(Buffer), &BytesRead);

			if (res != FR_OK)
				break;

			if (accelerometer_read(ACCEL_X) > 2200) {

				accel_volume_count++;

				if (accel_volume_count > 0x001F) {
					accel_volume_count = 0;
					accel_volume -= 0x0101;
				}

				led_on(LED1);
			}
			else led_off(LED1);

			if (accelerometer_read(ACCEL_X) < 1800) {

				accel_volume_count++;

				if (accel_volume_count > 0x001F) {
					accel_volume_count = 0;
					accel_volume += 0x0101;
				}

				led_on(LED3);
			} else
				led_off(LED3);

			if (accelerometer_read(ACCEL_Y) > 2200) {

				song_index--;
				led_on(LED4);
				break;
			} else
				led_off(LED4);

			if (accelerometer_read(ACCEL_Y) < 1800) {

				song_index++;
				led_on(LED2);
				break;
			} else
				led_off(LED2);

			if (button_getstate(BUTTON_S2) == 1) {

				printf("Entering SERVO Demo...\n");
				led_show_transition();
				while(button_getstate(BUTTON_S2) == 1);
				demo_mode = DEMO_SERVO;
				break;
			}

			audio_write_data((void*)(Buffer));

			if (BytesRead < sizeof(Buffer)) {

				song_index++;
				break;
			}

			if (accel_volume > 0xFEFE) accel_volume = 0xFEFE;
			if (accel_volume < 0x0101) accel_volume = 0x0101;

			audio_set_volume(accel_volume);
		}

		res = f_close(&fil);

		Sleep(1000);

		if (song_index < 0) song_index = song_total - 1;
		if (song_index > song_total -1 ) song_index = 0;
	}
	else {

		printf("error: reading microSD failed! (%i)\n", res);
		printf("skipping to SERVO Demo...\n");
		led_show_transition();
		demo_mode = DEMO_SERVO;

		return;
	}
}

void demo_servo(void)
{
	static int i = 100;
	static int dir = 0;

	if (dir) i++; else i--;
	if (i > 200) dir = 0;
	if (i < 100) dir = 1;

	servo_move(SERVO1, i);
	servo_move(SERVO2, i);
	servo_move(SERVO3, i);
	servo_move(SERVO4, i);
	servo_move(SERVO5, i);
	servo_move(SERVO6, i);
	servo_move(SERVO7, i);
	servo_move(SERVO8, i);
	servo_move(SERVO9, i);
	servo_move(SERVO10, i);
	servo_move(SERVO11, i);
	servo_move(SERVO12, i);
	servo_move(SERVO13, i);
	servo_move(SERVO14, i);
	servo_move(SERVO15, i);
	servo_move(SERVO16, i);
	servo_move(SERVO17, i);
	servo_move(SERVO18, i);
	servo_move(SERVO19, i);
	servo_move(SERVO20, i);
	servo_move(SERVO21, i);
	servo_move(SERVO22, i);
	servo_move(SERVO23, i);
	servo_move(SERVO24, i);
	servo_move(SERVO25, i);
	servo_move(SERVO26, i);
	Sleep(5);

	if (button_getstate(BUTTON_S2) == 1) {

		printf("Entering USB HID Demo...\n");
		led_show_transition();
		while(button_getstate(BUTTON_S2) == 1);
		demo_mode = DEMO_USB_HID;
	}
}

void demo_usb_hid(void)
{
	uint8_t mouse_buffer[4] = {0};

	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_HID_cb, &USR_cb);

	while(button_getstate(BUTTON_S2) == 0) {

		mouse_buffer[0] = 0;
		mouse_buffer[1] = 0;
		mouse_buffer[2] = 0;
		mouse_buffer[3] = 0;

		if (accelerometer_read(ACCEL_X) > 2200) {

			mouse_buffer[2] = (accelerometer_read(ACCEL_X) - 2200) / 8;

			led_on(LED1);
		} else
			led_off(LED1);

		if (accelerometer_read(ACCEL_X) < 1800) {

			accel_volume_count++;

			mouse_buffer[2] = (accelerometer_read(ACCEL_X) - 1800) / 8;

			led_on(LED3);
		} else
			led_off(LED3);

		if (accelerometer_read(ACCEL_Y) > 2200) {

			mouse_buffer[1] = (accelerometer_read(ACCEL_Y) - 2200) / 8;

			led_on(LED4);
		} else
			led_off(LED4);

		if (accelerometer_read(ACCEL_Y) < 1800) {

			mouse_buffer[1] = (accelerometer_read(ACCEL_Y) - 1800) / 8;

			led_on(LED2);
		} else
			led_off(LED2);

		USBD_HID_SendReport(&USB_OTG_dev, mouse_buffer, 4);

		Sleep(20);
	}

	/* Disconnect the USB device */
	DCD_DevDisconnect(&USB_OTG_dev);
	USB_OTG_StopDevice(&USB_OTG_dev);

	printf("Entering LED Demo...\n");
	led_show_transition();
	while(button_getstate(BUTTON_S2) == 1);

	demo_mode = DEMO_LED;
}

int setup_is_mp3(const char* fname)
{

	int name_len = 0;

	if (fname == 0 || strlen(fname) < 4) {
		return 0;
	}
	else {
		name_len = strlen(fname);
		return fname[name_len-1] == '3';
	}
}


int setup_load_music(const char* folder)
{
	// define local variables , because recursive
	FRESULT f_res;
	FILINFO f_info;
	DIR f_dir;
	char full_path[_MAX_LFN + 1];

	char *f_name;

	// open the folder
	res = f_opendir(&f_dir, folder);
	if (FR_OK != res) {
		printf("Open music folder %s Failed! ret = %d\n", folder, res);
	}

	printf(" |_ folder :%s ... \n", folder);

	for (;;) {
			f_res = f_readdir(&f_dir, &f_info); /* Read a directory item */

			if (f_res != FR_OK || f_info.fname[0] == 0) {
				break; /* Break on error or end of dir */
			}

			if (f_info.fname[0] == '.') {
				continue; /* Ignore dot entry */
			}

			f_name = f_info.fname;
			sprintf(full_path, "%s/%s", folder, f_name);

			if (f_info.fattrib & AM_DIR) { /* It is a directory */
				if (0 > setup_load_music(full_path)){
					break;
				}
			} else if (song_total >= NUMBER_OF_SONGS) { /* It is a file. */
				break;		// exceeded the max
			} else if (setup_is_mp3(f_name)) {
				printf("   |- file :%s \n", full_path);
				//songs[song_total].filename = (char *)malloc(strlen(full_path)+1);
				sprintf(songs[song_total].filename , "%s\n", full_path);
				song_total++;
			}
		}

		//f_closedir(&f_dir);

		return song_total >= NUMBER_OF_SONGS ? -1 : 0;
}

void setup(void) {
	printf("\nEntering demo setup:\n");

	/* Configure analog inputs for accelerometer reading */
	printf("configuring accelerometer...\n");
	accelerometer_config();

	/* Configure LEDs */
	printf("configuring LEDs...\n");
	led_config(LED1);
	led_config(LED2);
	led_config(LED3);
	led_config(LED4);

	/* Configure button S2 */
	printf("configuring button S2...\n");
	button_config(BUTTON_S2);

	/* Configure audio codec */
	printf("configuring audio driver...\n");
	audio_config();
	audio_set_volume(accel_volume);

	/* Loading songs list */
	printf("loading playlist...\n");
	setup_load_music("music");

	/* Configure servo motor ports */
	printf("configuring servo motors...\n");

	servo_move(SERVO1, 150);
	servo_move(SERVO2, 150);
	servo_move(SERVO3, 150);
	servo_move(SERVO4, 150);
	servo_move(SERVO5, 150);
	servo_move(SERVO6, 150);
	servo_move(SERVO7, 150);
	servo_move(SERVO8, 150);
	servo_move(SERVO9, 150);
	servo_move(SERVO10, 150);
	servo_move(SERVO11, 150);
	servo_move(SERVO12, 150);
	servo_move(SERVO13, 150);
	servo_move(SERVO14, 150);
	servo_move(SERVO15, 150);
	servo_move(SERVO16, 150);
	servo_move(SERVO17, 150);
	servo_move(SERVO18, 150);
	servo_move(SERVO19, 150);
	servo_move(SERVO20, 150);
	servo_move(SERVO21, 150);
	servo_move(SERVO22, 150);
	servo_move(SERVO23, 150);
	servo_move(SERVO24, 150);
	servo_move(SERVO25, 150);
	servo_move(SERVO26, 150);

	servo_config(SERVO1);
	servo_config(SERVO2);
	servo_config(SERVO3);
	servo_config(SERVO4);
	servo_config(SERVO5);
	servo_config(SERVO6);
	servo_config(SERVO7);
	servo_config(SERVO8);
	servo_config(SERVO9);
	servo_config(SERVO10);
	servo_config(SERVO11);
	servo_config(SERVO12);
	servo_config(SERVO13);
	servo_config(SERVO14);
	servo_config(SERVO15);
	servo_config(SERVO16);
	servo_config(SERVO17);
	servo_config(SERVO18);
	servo_config(SERVO19);
	servo_config(SERVO20);
	servo_config(SERVO21);
	servo_config(SERVO22);
	servo_config(SERVO23);
	servo_config(SERVO24);
	servo_config(SERVO25);
	servo_config(SERVO26);

	demo_led();

	printf("Press S2 to switch between demos!\n");
}

void loop(void)
{
	if (demo_mode == DEMO_LED) {
		demo_led();
	}

	if (demo_mode == DEMO_ACCELEROMETER) {
		demo_accelerometer();
	}

	if (demo_mode == DEMO_OGG_PLAYER) {
		demo_music_player();
	}

	if (demo_mode == DEMO_SERVO) {
		demo_servo();
	}

	if (demo_mode == DEMO_USB_HID) {
		demo_usb_hid();
	}
}

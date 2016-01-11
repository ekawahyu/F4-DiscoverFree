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

#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_hid_core.h"
#include "usbd_cdc_vcp.h"

USB_OTG_CORE_HANDLE USB_OTG_FS_dev;

void setup(void) {

	USBD_Init(&USB_OTG_FS_dev, USB_OTG_FS_CORE_ID, &USR_CDC_desc, &USBD_CDC_cb, &USR_cb);
	//USBD_Init(&USB_OTG_FS_dev, USB_OTG_FS_CORE_ID, &USR_HID_desc, &USBD_HID_cb, &USR_cb);

	led_config(LED1);
	led_config(LED2);
	led_config(LED3);
	led_config(LED4);
}

void loop(void)
{
	static int i;
	char answer;
	uint8_t mouse_buffer[4] = {0, 10, 10, 0};

	printf("Enter LED number: ");
	answer = getchar();
	printf("\n");

	if (answer == '1') led_toggle(LED1); /* enter 1 to turn on LED1 */
	if (answer == '2') led_toggle(LED2); /* enter 2 to turn on LED2 */
	if (answer == '3') led_toggle(LED3); /* enter 3 to turn on LED3 */
	if (answer == '4') led_toggle(LED4); /* enter 4 to turn on LED4 */

	//USBD_HID_SendReport(&USB_OTG_FS_dev, mouse_buffer, 4);

	Sleep(10);

	//printf("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789-0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789-0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789-0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789-0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789(%i)", i, i++);

	/* Disconnect the USB CDC device */
	//DCD_DevDisconnect(&USB_OTG_CDC_dev);
	//USB_OTG_StopDevice(&USB_OTG_CDC_dev);

	/* Disconnect the USB HID device */
	//DCD_DevDisconnect(&USB_OTG_HID_dev);
	//USB_OTG_StopDevice(&USB_OTG_HID_dev);
}

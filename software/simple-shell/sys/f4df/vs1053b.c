/*
 * vs1053b.c
 *
 * Created on: Nov 26, 2012
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

#include "stm32f4xx.h"
#include "vs1053b.h"

#define VS1053B_SPEED_SLOW			0
#define VS1053B_SPEED_HIGH			1

#define VS1053B_WRITE_COMMAND		0x02
#define VS1053B_READ_COMMAND		0x03

#define VS1053B_CCS(x)			x ? GPIO_SetBits(GPIOD,GPIO_Pin_3) : GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define VS1053B_DCS(x)			x ? GPIO_SetBits(GPIOC,GPIO_Pin_2) : GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define VS1053B_DREQ			GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)

void vs1053b_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	/* Enable GPIO and SPI2 clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	/* Configure and connect SPI pins: SCK, MISO and MOSI to AF5 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure XCS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure XDCS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure DREQ pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	VS1053B_DCS(1);
	VS1053B_CCS(1);

	/* SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI2 */
	SPI_Cmd(SPI2, ENABLE);
}

void vs1053b_deconfig(void)
{
	/* not used at the moment, to be defined */
}

void vs1053b_set_speed( uint8_t speed)
{
	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;//SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;//SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;

	if( speed == VS1053B_SPEED_SLOW ) {
		SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
	}
	else {
		SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
	}

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStructure);
}

uint8_t vs1053b_read_write(uint8_t data)
{
	/* Loop while DR register in not empty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* Send one byte through SPI */
	SPI_I2S_SendData(SPI2, data);

	/* Wait to receive one byte from SPI */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the data read */
	return SPI_I2S_ReceiveData(SPI2);
}

void vs1053b_write_reg(uint8_t reg, uint16_t value)
{
	while(VS1053B_DREQ == 0);

	vs1053b_set_speed(VS1053B_SPEED_SLOW);

	VS1053B_DCS(1);
	VS1053B_CCS(0);

	vs1053b_read_write(VS1053B_WRITE_COMMAND);
	vs1053b_read_write(reg);
	vs1053b_read_write(value >> 8);
	vs1053b_read_write(value);

	VS1053B_CCS(1);

	vs1053b_set_speed(VS1053B_SPEED_HIGH);
}

void vs1053b_write_data(uint8_t * buf)
{
	uint8_t count = 32;

	while(VS1053B_DREQ == 0);

	VS1053B_DCS(0);
	while(count--) vs1053b_read_write(*buf++);

	VS1053B_DCS(1);
	VS1053B_CCS(1);
}

uint16_t vs1053b_read_reg(uint8_t reg)
{
	uint16_t value;

	while(VS1053B_DREQ == 0);

	vs1053b_set_speed(VS1053B_SPEED_SLOW);

	VS1053B_DCS(1);
	VS1053B_CCS(0);

	vs1053b_read_write(VS1053B_READ_COMMAND);
	vs1053b_read_write(reg);
	value = vs1053b_read_write(0xFF);
	value = value << 8;
	value |= vs1053b_read_write(0xFF);

	VS1053B_CCS(1);

	vs1053b_set_speed(VS1053B_SPEED_HIGH);

	return value;
}

void vs1053b_soft_reset(void)
{
	vs1053b_write_reg(SPI_MODE, 0x0804);

	vs1053b_write_reg(SPI_CLOCKF, 0x9800); /* xtal at 12.288MHz */

	vs1053b_write_reg(SPI_AUDATA, 0xBB81); /* 48kHz stereo */

	vs1053b_write_reg(SPI_DECODE_TIME, 0x0000);

	vs1053b_write_reg(SPI_DECODE_TIME, 0x0000);
}

void vs1053b_set_volume(uint16_t vol)
{
	/* analog power down set to 0xFFFF; max volume at 0x0000 */
	vs1053b_write_reg(SPI_VOL, vol);
}

uint16_t vs1053b_get_volume(void)
{
	return vs1053b_read_reg(SPI_VOL);
}

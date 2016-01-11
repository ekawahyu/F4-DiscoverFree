/*
 * vs1053b.h
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

#ifndef VS1053B_H_
#define VS1053B_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_MODE			0x00
#define SPI_STATUS			0x01
#define SPI_BASS			0x02
#define SPI_CLOCKF			0x03
#define SPI_DECODE_TIME		0x04
#define SPI_AUDATA			0x05
#define SPI_WRAM			0x06
#define SPI_WRAMADDR		0x07
#define SPI_HDAT0			0x08
#define SPI_HDAT1			0x09
#define SPI_AIADDR			0x0A
#define SPI_VOL				0x0B
#define SPI_AICTRL0			0x0C
#define SPI_AICTRL1			0x0D
#define SPI_AICTRL2			0x0E
#define SPI_AICTRL3			0x0F

void vs1053b_config(void);
void vs1053b_deconfig(void);
uint16_t vs1053b_read_reg(uint8_t reg);
void vs1053b_write_data(uint8_t * buf);
void vs1053b_soft_reset(void);
void vs1053b_set_volume(uint16_t vol);
uint16_t vs1053b_get_volume(void);
void vs1053b_sine_test(void);

#ifdef __cplusplus
}
#endif

#endif /* VS1053B_H_ */

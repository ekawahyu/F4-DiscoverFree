/*
 * adc.c
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

#include "adc.h"

#define ADC1_DR_ADDRESS     ((uint32_t)0x4001204C)
#define ADC2_DR_ADDRESS     ((uint32_t)0x4001214C)
#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)

__IO uint16_t accelerometer_value_x = 2000;
__IO uint16_t accelerometer_value_y = 2000;
__IO uint16_t accelerometer_value_z = 2000;

__IO uint8_t adc1_configured = 0;
__IO uint8_t adc3_configured = 0;

uint16_t analog_pin[ANALOGn]       = {
		ANALOG1_PIN,  ANALOG2_PIN,  ANALOG3_PIN,  ANALOG4_PIN,
		ANALOG5_PIN,  ANALOG6_PIN,  ANALOG7_PIN,  ANALOG8_PIN,
		ANALOG9_PIN,  ANALOG10_PIN, ANALOG11_PIN, ANALOG12_PIN,
		ANALOG13_PIN, ANALOG14_PIN, ANALOG15_PIN, ANALOG16_PIN,
		ANALOG17_PIN, ANALOG18_PIN, ANALOG19_PIN, ANALOG20_PIN,
		ANALOG21_PIN, ANALOG22_PIN, ANALOG23_PIN, ANALOG24_PIN
};

GPIO_TypeDef* analog_gpio[ANALOGn] = {
		ANALOG1_GPIO,  ANALOG2_GPIO,  ANALOG3_GPIO,  ANALOG4_GPIO,
		ANALOG5_GPIO,  ANALOG6_GPIO,  ANALOG7_GPIO,  ANALOG8_GPIO,
		ANALOG9_GPIO,  ANALOG10_GPIO, ANALOG11_GPIO, ANALOG12_GPIO,
		ANALOG13_GPIO, ANALOG14_GPIO, ANALOG15_GPIO, ANALOG16_GPIO,
		ANALOG17_GPIO, ANALOG18_GPIO, ANALOG19_GPIO, ANALOG20_GPIO,
		ANALOG21_GPIO, ANALOG22_GPIO, ANALOG23_GPIO, ANALOG24_GPIO
};

uint32_t analog_gpio_clk[ANALOGn]  = {
		ANALOG1_GPIO_CLK,  ANALOG2_GPIO_CLK,  ANALOG3_GPIO_CLK,  ANALOG4_GPIO_CLK,
		ANALOG5_GPIO_CLK,  ANALOG6_GPIO_CLK,  ANALOG7_GPIO_CLK,  ANALOG8_GPIO_CLK,
		ANALOG9_GPIO_CLK,  ANALOG10_GPIO_CLK, ANALOG11_GPIO_CLK, ANALOG12_GPIO_CLK,
		ANALOG13_GPIO_CLK, ANALOG14_GPIO_CLK, ANALOG15_GPIO_CLK, ANALOG16_GPIO_CLK,
		ANALOG17_GPIO_CLK, ANALOG18_GPIO_CLK, ANALOG19_GPIO_CLK, ANALOG20_GPIO_CLK,
		ANALOG21_GPIO_CLK, ANALOG22_GPIO_CLK, ANALOG23_GPIO_CLK, ANALOG24_GPIO_CLK
};

uint8_t analog_channel[ANALOGn] = {
		ANALOG1_CHANNEL,  ANALOG2_CHANNEL,  ANALOG3_CHANNEL,  ANALOG4_CHANNEL,
		ANALOG5_CHANNEL,  ANALOG6_CHANNEL,  ANALOG7_CHANNEL,  ANALOG8_CHANNEL,
		ANALOG9_CHANNEL,  ANALOG10_CHANNEL, ANALOG11_CHANNEL, ANALOG12_CHANNEL,
		ANALOG13_CHANNEL, ANALOG14_CHANNEL, ANALOG15_CHANNEL, ANALOG16_CHANNEL,
		ANALOG17_CHANNEL, ANALOG18_CHANNEL, ANALOG19_CHANNEL, ANALOG20_CHANNEL,
		ANALOG21_CHANNEL, ANALOG22_CHANNEL, ANALOG23_CHANNEL, ANALOG24_CHANNEL
};

void adc1_config(void)
{
	ADC_InitTypeDef       ADC_InitStructure = {0};
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	if (adc1_configured) return;
	else adc1_configured = 1;

	/* Enable ADC1 clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC common init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

void adc3_config(void)
{
	ADC_InitTypeDef       ADC_InitStructure = {0};
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	if (adc3_configured) return;
	else adc3_configured = 1;

	/* Enable ADC3 clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* ADC common init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	ADC_EOCOnEachRegularChannelCmd(ADC3, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
}

uint16_t read_adc1(uint8_t channel)
{
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_3Cycles);

	//Clear flag before conversion begins
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

	// Start the conversion
	ADC_SoftwareStartConv(ADC1);

	// Wait until conversion completion
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	// Get the conversion value
	return ADC_GetConversionValue(ADC1);
}

uint16_t read_adc3(uint8_t channel)
{
	ADC_RegularChannelConfig(ADC3, channel, 1, ADC_SampleTime_3Cycles);

	//Clear flag before conversion begins
	ADC_ClearFlag(ADC3, ADC_FLAG_EOC);

	// Start the conversion
	ADC_SoftwareStartConv(ADC3);

	// Wait until conversion completion
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);

	// Get the conversion value
	return ADC_GetConversionValue(ADC3);
}

int16_t analog_config(analog_t analog)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Capture wrong analog number */
	if (analog >= ANALOGn) return -1;

	if (analog >= ANALOG17)
		adc3_config();
	else
		adc1_config();

	/* Enable GPIO peripheral clock */
	RCC_AHB1PeriphClockCmd(analog_gpio_clk[analog], ENABLE);

	/* Configure ANALOG pin as analog input */
	GPIO_InitStructure.GPIO_Pin = analog_pin[analog];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(analog_gpio[analog], &GPIO_InitStructure);

	return 0;
}

int16_t analog_deconfig(analog_t analog)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure unused GPIO port pins in Analog Input mode (floating input
	 * trigger OFF), this will reduce the power consumption and increase the
	 * device immunity against EMI/EMC */

	/* Capture wrong led number */
	if (analog >= ANALOGn) return -1;

	/* Configure ANALOG pin as analog input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = analog_pin[analog];
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(analog_gpio[analog], &GPIO_InitStructure);

	return 0;
}

int16_t analog_read(analog_t analog)
{
	/* Capture wrong led number */
	if (analog >= ANALOGn) return -1;

	if (analog >= ANALOG17)
		return read_adc3(analog_channel[analog]);
	else
		return read_adc1(analog_channel[analog]);
}

void ADC1_CH8_DMA_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure = {0};
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC1, DMA2 and GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* DMA2 stream 0 channel 0 configuration */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&accelerometer_value_x;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure PB0 pin as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ADC common init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel 8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

void ADC2_CH9_DMA_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure = {0};
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC2, DMA2 and GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

	/* DMA2 stream 2 channel 1 configuration */
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC2_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&accelerometer_value_y;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream2, ENABLE);

	/* Configure PB1 pin as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ADC common init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC2 init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC2, &ADC_InitStructure);

	/* ADC2 regular channel 9 configuration */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer */
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);

	/* Enable ADC2 DMA */
	ADC_DMACmd(ADC2, ENABLE);

	/* Enable ADC2 */
	ADC_Cmd(ADC2, ENABLE);
}

void ADC3_CH3_DMA_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure = {0};
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC3, DMA2 and GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* DMA2 stream 1 channel 2 configuration */
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&accelerometer_value_z;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream1, ENABLE);

	/* Configure PA3 pin as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ADC common init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel 3 configuration */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer */
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
}

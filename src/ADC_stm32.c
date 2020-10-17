/*

 * ADC_stm32.c
 *
 *  Created on: 17.10.2020
 *      Author: mrhause
 */
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

#define ADC_PIN_PORT GPIOA
#define ADC_PIN GPIO_Pin_0

static void ADC_GPIO_Config();

void ADC_Config(){
	ADC_GPIO_Config(); //configure PIN

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitTypeDef hadc;
	ADC_StructInit(&hadc);
	hadc.ADC_ContinuousConvMode = DISABLE;
	hadc.ADC_NbrOfChannel = 1;
	hadc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC1, &hadc);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

}

static void ADC_GPIO_Config(){
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = ADC_PIN;
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PIN_PORT, &gpio);
}

uint32_t ADC_get_value(uint8_t channel){
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_13Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//start conversion

	while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET ); //wait for conversion end

	return ADC_GetConversionValue( ADC1 );	//return value
}


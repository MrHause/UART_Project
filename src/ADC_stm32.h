/*
 * ADC_stm32.h
 *
 *  Created on: 17.10.2020
 *      Author: mrhau
 */

#ifndef ADC_STM32_H_
#define ADC_STM32_H_

void ADC_Config();

uint32_t ADC_get_value(uint8_t channel);

#endif /* ADC_STM32_H_ */

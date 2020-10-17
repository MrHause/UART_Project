/*
 * UART_stm32.h
 *
 *  Created on: 07.10.2020
 *      Author: mrhau
 */

#ifndef UART_STM32_H_
#define UART_STM32_H_

#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

//#define UART_PORT GPIOA
//#define UART_NUM USART2
//#define UART_PIN_RX GPIO_Pin_3
//#define UART_PIN_TX GPIO_Pin_2

void UART_Config( USART_TypeDef* USARTx, GPIO_TypeDef* UART_PORT, uint16_t UART_PIN_TX, uint16_t UART_PIN_RX );

void UART_Config_receive_IRQ( USART_TypeDef* USARTx, IRQn_Type interrupt );

void send_char( USART_TypeDef* UART_NUM, const char sign );

void send_string( USART_TypeDef* UART_NUM, const char* str );

#endif /* UART_STM32_H_ */

/*
 * UART_stm32.c
 *
 *  Created on: 07.10.2020
 *      Author: mrhau
 */

#include "UART_stm32.h"
#include "ring_buffer.h"


void UART_Config(USART_TypeDef* UART_NUM, GPIO_TypeDef* UART_PORT, uint16_t UART_PIN_TX, uint16_t UART_PIN_RX ){
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = UART_PIN_TX;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART_PORT, &gpio);

	gpio.GPIO_Pin = UART_PIN_RX;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART_PORT, &gpio);

	USART_StructInit(&uart);
	uart.USART_BaudRate = 115200;
	USART_Init(UART_NUM, &uart);

	USART_Cmd(UART_NUM, ENABLE);
}

void UART_Config_receive_IRQ( USART_TypeDef* USARTx, IRQn_Type interrupt ){
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(interrupt);
}


void send_char( USART_TypeDef* UART_NUM, const char sign ){
	while( USART_GetFlagStatus(UART_NUM, USART_FLAG_TXE) == RESET );
	USART_SendData( UART_NUM, sign );
}

void send_string(USART_TypeDef* UART_NUM, const char* str ){
	while(*str){
		send_char( UART_NUM, *str );
		str++;
	}
}


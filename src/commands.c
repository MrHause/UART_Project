/*
 * commands.c

 *
 *  Created on: 11.10.2020
 *      Author: mrhause
 */

#include "stm32f10x.h"
#include "commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "UART_stm32.h"
#include "ADC_stm32.h"

static void command_led_toogle( uint8_t state );
static void command_adc_get_value( uint8_t channel );

Commands_list command_analyse( Ring_buffer_type *buffer ){
	Commands_list command = NO_COMMAND;
	uint8_t *ptr = &buffer->ring_buff[buffer->tail];
	uint8_t *message;
	message = malloc( sizeof(buffer->ring_buff) );
	memset( message, 0, sizeof(buffer->ring_buff) );
	uint8_t it = 0;
	ptr++;
	while( *ptr ){
		message[it] = Ring_buffer_get_item( buffer );
		it++;
		if( buffer->tail == (buffer->size-1) )
			ptr = buffer->ring_buff;	//set at the beginning
		else
			ptr++;
	}

	if( !strcmp( message, "LED_ON\r" ) ){
		command = LED_ON;
	}
	else if( !strcmp( message, "LED_OFF\r" ) )
		command = LED_OFF;
	else if( !strcmp( message, "ADC_GET_VALUE\r" ) )
		command = ADC_GET_VALUE;

	free(message);
	return command;
}

void command_execute( Commands_list command ){
	switch(command){
	case LED_ON:
		command_led_toogle( 0 );
		break;
	case LED_OFF:
		command_led_toogle( 1 );
		break;
	case ADC_GET_VALUE:
		command_adc_get_value( ADC_Channel_0 );
	default:
		break;
	}
}

static void command_led_toogle(uint8_t state){
	if( !state )
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	else
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}

static void command_adc_get_value(uint8_t channel){
	uint32_t value;
	char to_send[10];
	value = ADC_get_value( channel );

	float voltage = (float)value * 3.3f / 4096.0f;
	sprintf(to_send, "%.2f", voltage);

	send_string(USART2, to_send);
}


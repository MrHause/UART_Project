/*
 * commands.c

 *
 *  Created on: 11.10.2020
 *      Author: mrhause
 */

#include "commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void led_toogle(uint8_t state);

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

	free(message);
	return command;
}

void command_execute( Commands_list command ){
	switch(command){
	case LED_ON:
		led_toogle( 0 );
		break;
	case LED_OFF:
		led_toogle( 1 );
		break;
	default:
		break;
	}
}

static void led_toogle(uint8_t state){
	if( !state )
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	else
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}



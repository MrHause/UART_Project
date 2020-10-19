/*
 * commands.h
 *
 *  Created on: 11.10.2020
 *      Author: mrhau
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "ring_buffer.h"

typedef enum{
	NO_COMMAND = 0,
	LED_ON,
	LED_OFF,
	ADC_GET_VALUE,
	TMP102_TEMP
}Commands_list;

Commands_list command_analyse( Ring_buffer_type *buffer );

void command_execute( Commands_list command );

#endif /* COMMANDS_H_ */

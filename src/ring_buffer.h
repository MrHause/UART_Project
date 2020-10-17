/*
 * ring_buffer.h
 *
 *  Created on: 09.10.2020
 *      Author: mrhau
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

#define RING_BUFFER_MAX_SIZE 512

typedef struct{
	uint8_t ring_buff[RING_BUFFER_MAX_SIZE];
	uint16_t size;
	uint16_t head;
	uint16_t tail;
}Ring_buffer_type;

void Ring_buffer_init( Ring_buffer_type *buffer, uint32_t size );

void Ring_buffer_add_item( Ring_buffer_type *buffer, const char element );

char Ring_buffer_get_item( Ring_buffer_type *buffer );

uint8_t Ring_buffer_is_full( Ring_buffer_type *buffer );

uint8_t Ring_buffer_is_sth_to_read( Ring_buffer_type *buffer );

void Ring_buffer_add__multi_item( Ring_buffer_type *buffer, const char *data, uint16_t bytes );

void Ring_buffer_reset( Ring_buffer_type *buffer );


#endif /* RING_BUFFER_H_ */

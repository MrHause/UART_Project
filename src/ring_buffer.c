/*
 * ring_buffer.c
 *
 *  Created on: 09.10.2020
 *      Author: mrhause
 */
/*
 * Ring buffer for uint8_t (char) elements.
 *
 */
#include "ring_buffer.h"
#include <stdio.h>
#include <string.h>

void Ring_buffer_init( Ring_buffer_type *buffer, uint32_t size ){
	buffer->size = size;
	buffer->head = 0;
	buffer->tail = 0;
}

void Ring_buffer_add_item( Ring_buffer_type *buffer, const char element ){
	uint16_t tmp_head;
	tmp_head = buffer->head;
	tmp_head = (buffer->head+1) & (buffer->size-1);
	buffer->ring_buff[tmp_head] = element;
	buffer->head = tmp_head;
}

char Ring_buffer_get_item( Ring_buffer_type *buffer ){
	char element;

	if( buffer->head == buffer->tail )	//there is nothing to read
		return 0;

	buffer->tail = (buffer->tail + 1) & (buffer->size - 1);

	element = buffer->ring_buff[buffer->tail];

	return element;
}

uint8_t Ring_buffer_is_full( Ring_buffer_type *buffer ){
	if( buffer->head >= buffer->size )
		return 1;	//is full
	else
		return 0;	//is not full
}

uint8_t Ring_buffer_is_sth_to_read( Ring_buffer_type *buffer ){
	if( buffer->head != buffer->tail )
		return 0;
	else
		return 1;
}

void Ring_buffer_add__multi_item( Ring_buffer_type *buffer, const char *data, uint16_t bytes ){
	uint16_t buff_free_space = 0, tmp_head = 0;
	tmp_head = buffer->head;
	tmp_head = (buffer->head+1) & (buffer->size-1);	//move pointer, if end move at the beggining
	buff_free_space = buffer->size - buffer->head;	//get free space
	if( bytes >= buff_free_space ){					//to many bytes, must be split in two regions
		memcpy( &buffer->ring_buff[tmp_head], data, buff_free_space );
		bytes -= buff_free_space;					//calculate rest data to save in buffer
		data += buff_free_space;					//move pointer
		tmp_head = 0;								//set pointer to the beggining
	}
	if(bytes > 0)
		memcpy( &buffer->ring_buff[tmp_head], data, bytes );

	tmp_head += bytes;
	buffer->head = tmp_head;
}

void Ring_buffer_reset( Ring_buffer_type *buffer ){
	memset( buffer->ring_buff, 0, buffer->size );
	buffer->head = 0;
	buffer->tail = 0;
}

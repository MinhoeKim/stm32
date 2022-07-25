/*
 * callqueue.c
 *
 *  Created on: Jul 14, 2022
 *      Author: 1
 */

#include "callqueue.h"

uint16_t u3_rx_buffer[U3_BUFFER_SIZE];
uint32_t u3_rx_point_head = 0;
uint32_t u3_rx_point_tail = 0;

void Uart3_EnQueue(uint16_t data)
{
	u3_rx_buffer[u3_rx_point_head] = data;
	u3_increase_point_value(&u3_rx_point_head);
}


void u3_increase_point_value(uint32_t * data_p)
{
	(*data_p)++;
	if(U3_BUFFER_SIZE == (*data_p))
	{
		(*data_p)=0;
	}
}


uint16_t Uart3_DeQueue(void)
{
	uint16_t retVal = u3_rx_buffer[u3_rx_point_tail];
	u3_increase_point_value(&u3_rx_point_tail);
	return retVal;
}

bool Uart3_Is_Empty(void)
{
	if(u3_rx_point_head == u3_rx_point_tail)
	{
		return TURE;
	}
	return FALSE;
}

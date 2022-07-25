/*
 * callqueue.h
 *
 *  Created on: Jul 14, 2022
 *      Author: 1
 */

#ifndef INC_CALLQUEUE_H_
#define INC_CALLQUEUE_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define U3_BUFFER_SIZE 100
#define TURE 1
#define FALSE 0
void Uart3_EnQueue(uint16_t data);
void u3_increase_point_value(uint32_t *data_p);
uint16_t Uart3_DeQueue(void);
bool Uart3_IsEmpty(void);

#endif /* INC_CALLQUEUE_H_ */

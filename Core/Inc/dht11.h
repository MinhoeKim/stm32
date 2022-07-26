/*
 * dht11.h
 *
 *  Created on: Jul 25, 2022
 *      Author: 1
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include<stdlib.h>
#include<stdint.h>
#include"stm32f4xx.h"
#include"cmsis_os.h"
#include"FreeRTOS.h"
#include"task.h"
#include"stdbool.h"

#define printf SMprintf
#define MAXTIMINGS 83
#define DHTPIN GPIO_PIN_8
#define HIGH 1
#define LOW 0
#define INPUT_MODE 0
#define OUTPUT_MODE 1
void Change_GPIOMode(int mode);
extern void delayMicroseconds(int time);
void read_dht11_dat();

#endif /* INC_DHT11_H_ */

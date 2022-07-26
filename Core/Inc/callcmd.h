/*
 * callcmd.h
 *
 *  Created on: Jul 14, 2022
 *      Author: 1
 */

#ifndef INC_CALLCMD_H_
#define INC_CALLCMD_H_


#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"string.h"
#include"stm32f4xx_hal.h"
#include "cmsis_os.h"
#include"FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "dwt_stm32_delay.h"
#include<stdbool.h>

#define byte unsigned char
#define uint8 unsigned char
#define word unsigned short
#define lword unsigned int
#define int8u unsigned char
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define DD_SUCCESS          YES
#define FALSE -1
#define YES 1
#define NO 0

#define COM1_DEBUG 0x00
#define printf SMprintf
uint16_t Uart3_DeQueue(void);
bool Uart3_Is_Empty(void);
bool Uart3_Is_Empty(void);
extern UART_HandleTypeDef huart3; //access huart3 instance
extern UART_HandleTypeDef huart3; //access huart3 instance

void prt_hexbnb( int n, int8u *hex);
int8u ktolower(int8u c);
void cpybuf(int8u *src, int8u *des,int cnt);
int get_nstring(char buf[]);
char get_io_nember(unsigned int *retval);
int dsp_mem(int argc,char*argv[]);
int chg_mem(int argc,char *argv[]);
int mem_read(int argc,char *argv[]);
int fill(int8u *sp,int8u c, int n);
int parse_string(unsigned char *s,char *argv[]);
int wrd_mon(int argc,char *argv[]);
int bfm_msg_mon(int argc,char *argv[]);
void onl_monitor_chelp(int argc,char *argv[]);
int wl_mon(int argc,char *argv[]);
int wr_loop(int argc,char *argv[]);

int dsp_mem(int argc,char*argv[]);

int DHT11(int argc,char *argv[]);

int AlarmSet(int argc,char *argv[]);

int SETTime(int argc, char *argv[]);

int DisTime(int argc,char *argv[]);

int RTCTest(int argc,char *argv[]);

#endif /* INC_CALLCMD_H_ */

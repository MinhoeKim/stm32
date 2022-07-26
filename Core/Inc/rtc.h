/*
 * rtc.h
 *
 *  Created on: Jul 26, 2022
 *      Author: 1
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#define printf SMprintf
#define NO 0
#ifdef __cplusplus
extern"C"{
#endif

#define Error_Handler() _Error_Handler(__FILE__,__LINE__)



void set_time(void);
void set_time_only(unsigned char hour, unsigned char minute, unsigned char second);
void set_time_date(void);
void set_alarm(unsigned char hour, unsigned char minite, unsigned char second);
void get_time(void);
void rtc_test(void);







#ifdef __cplusplus
}
#endif
extern bool temp_flag;
extern unsigned char tmep_data[4];


#endif /* INC_RTC_H_ */

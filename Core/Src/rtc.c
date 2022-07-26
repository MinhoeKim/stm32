#include "main.h"
#include"rtc.h"
#include"stm32f4xx_hal.h"
#include "cmsis_os.h"

extern RTC_HandleTypeDef hrtc;

char time[16];
char date[16];
extern unsigned char AlarmFlag = 0;
extern RTC_AlarmTypeDef sAlarm;


void set_time(void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	sTime.Hours = 0x10;
	sTime.Minutes = 0x44;
	sTime.Seconds = 0x30;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	if(HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BCD)!=HAL_OK)
	{
		printf("Error RTC Settime\n\r");
	}

	sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
	sDate.Month = RTC_MONTH_AUGUST;
	sDate.Date = 0x23;
	sDate.Year = 0x21;

	if(HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BCD)!=HAL_OK)
	{
		printf("Error RTC Settime\n\r");
	}

	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0x32F2);

}

void set_time_only(unsigned char hour, unsigned char minute, unsigned char second)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;


	sTime.Hours = hour;
	sTime.Minutes = minute;
	sTime.Seconds = second;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if(HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BCD)!= HAL_OK)
	{
		printf("Error RTC Settime\n\r");
	}
}

void set_time_date(void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	sTime.Hours = 0x10;
	sTime.Minutes = 0x44;
	sTime.Seconds = 0x30;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if(HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BCD)!=HAL_OK)
	{
		printf("Error RTC Settime\n\r");
	}

	sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
	sDate.Month = RTC_MONTH_AUGUST;
	sDate.Date = 0x23;
	sDate.Year = 0x21;

	if(HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BCD)!=HAL_OK)
	{
		printf("Error RTC Settime\n\r");
	}


}

void set_alarm(unsigned char hour, unsigned char minite, unsigned char second)
{
	RTC_AlarmTypeDef sAlarm;

	sAlarm.AlarmTime.Hours = hour;
	sAlarm.AlarmTime.Minutes = minite;
	sAlarm.AlarmTime.Seconds = second;
	sAlarm.AlarmTime.SubSeconds = 0x0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;

	sAlarm.AlarmSubSecondMask = RTC_ALARMMASK_DATEWEEKDAY;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 0x1;
	sAlarm.Alarm = RTC_ALARM_A;

	if(HAL_RTC_SetAlarm_IT(&hrtc,&sAlarm,RTC_FORMAT_BCD)!=HAL_OK)
	{
		printf("Error RTC Settime\n\r");
	}
}

void get_time(void)
{
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;

	HAL_RTC_GetTime(&hrtc,&gTime,RTC_FORMAT_BIN);

	HAL_RTC_GetDate(&hrtc,&gDate,RTC_FORMAT_BIN);

	printf("20%02d-%20d-%20d  ",gDate.Year,gDate.Month,gDate.Date);
	printf("%02d:%02d:%02d\r",gTime.Hours,gTime.Minutes,gTime.Seconds);
}

void rtc_test(void)
{
	RTC_AlarmTypeDef sAlarm;
	int retval;

	retval = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
	printf("RTCEx_BKUPRead = %x\r\n",retval);

	if(retval != 0x32F2)
	{
		set_time();
	}
	printf("Alarm Time %02x-%02x-%02x\r\n",sAlarm.AlarmTime.Hours,sAlarm.AlarmTime.Minutes,sAlarm.AlarmTime.Seconds);

	while(1)
	{
		get_time();

		osDelay(1000);
		if(AlarmFlag)
		{
			HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET);
			printf("Alarm Time %02x-%02x-%02x\r\n",sAlarm.AlarmTime.Hours,sAlarm.AlarmTime.Minutes,sAlarm.AlarmTime.Seconds);
			AlarmFlag = NO;
			break;
		}
	}
}

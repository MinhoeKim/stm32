#include "dht11.h"

int dht11_dat[5] = {0,};
unsigned char temp_data[4];
bool temp_flag;

void read_dht11_dat()
{
	unsigned char laststate = HIGH;
	unsigned char counter = 0;
	unsigned char j = 0, i;
	unsigned char flag = HIGH;
	unsigned char state = 0;
	float f;
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	Change_GPIOMode(OUTPUT_MODE);
	HAL_GPIO_WritePin(GPIOB,DHTPIN,LOW);
	osDelay(18);
	HAL_GPIO_WritePin(GPIOB,DHTPIN,HIGH);
	delayMicroseconds(40);
	Change_GPIOMode(INPUT_MODE);
	for(i = 0; i < MAXTIMINGS ; i++)
	{
		counter = 0;
		while(HAL_GPIO_ReadPin(GPIOB,DHTPIN)==laststate)
		{
			counter++;
			delayMicroseconds(1);
			if(counter == 200)
				break;
		}
		laststate = HAL_GPIO_ReadPin(GPIOB,DHTPIN);
		if(counter == 200)
			break; //if while breaked by timer,break for
		if((i>=4)&&(i%2 == 0))
		{
			dht11_dat[j/8]<<=1;
			if(counter > 20)dht11_dat[j/8] |=1;
			j++;
		}
	}
	if((j>=40)||(dht11_dat[4] == ((dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3])&0xff)))
	{
		printf("humidity = %d.%d %% Temperature = %d.%d *C\n\r",dht11_dat[0],dht11_dat[1],dht11_dat[2],dht11_dat[3]);
		for(i=0;i<4;i++)
		{
			temp_data[i] = dht11_dat[i];
			temp_flag = HIGH;
		}
	}
	else
	{
		printf("Data get failed\n\r");
	}
}

void Change_GPIOMode(int mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(mode == INPUT_MODE)
	{
		GPIO_InitStruct.Pin = DHTPIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	}
	else
	{
		GPIO_InitStruct.Pin = DHTPIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	}
}

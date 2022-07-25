/*
 * callcmd.c
 *
 *  Created on: Jul 14, 2022
 *      Author: 1
 */


#include "callcmd.h"
#include"freeRTOS.h"
#include"queue.h"
#include<stdarg.h>
#include <errno.h>
#include <sys/unistd.h>

typedef struct
{
	char *str;
	int (*func)(int,char**);
	char *useage;
}Cmd_tbl;


extern const Cmd_tbl cmd_ctbl[];
char str_buff[512];
void SMprintf(const char *format,...)
{
	va_list args;
	char *str;


	str=str_buff;
	va_start(args,format);
	vsprintf(str,format,args);
	va_end(args);
	HAL_UART_Transmit(&huart3,(unsigned char*)str,strlen(str),10);
}

int8u ktolower(int8u c)
{
	if(c>='A'&& c <='Z')
		c-='A'-'a';
	return(c);
}

void cpybuf(int8u *src,int8u *des,int cnt)
{
	while(--cnt >=0)
		*src++ = *des++;
}

//Discription : compare strings with length
//output : 1) s1!= s2 : 1
//		2)s1==s2 : 0

int kstrncmp(char *s1,char *s2,int len)
{
	if(s1 == s2) return(0);
	while(len--){
		if(*s1++ != *s2++)
		{
			return(1);
		}
		if(*s1 == '\0' && *s2 == '\0')
		{
			break;
		}
	}
	return(0);
}


int atoh(char *s)
{
	register int i,n;

	for(n = i = 0 ; s[i] != '\0' ; i++)
	{
		if(s[i] >='0' && s[i] <='9')
			n = 16*n+s[i] - '0';
		else if(s[i]>='a'&&s[i]<'f')
			n = 16*n+s[i] - 'a'+10 ;
		else if(s[i]>='A'&& s[i] <='F')
			n = 16*n + s[i] - 'A'+10;
		else
			return(-1);
	}
	return (n);
}
// 함수:USART에서 1개의 CHAR 를 읽어 오는 함수
// input: Uart3 Queue
// output :: c[0] Uart3에서 읽은 char

char get_char_os(char c[])
{

	for(;;)
	{
		if(!Uart3_Is_Empty())
		{
			c[0] = Uart3_DeQueue();
			return YES;
		}
		else
		{
			osDelay(50);
			continue;
		}
	}
	return(1);
}

//char tbuf[5],omon_buf[80];
//int  onch_no = 0;

int bfm_msg_mon(int argc,char *argv[])
{
	printf("bfmok");
	return(1);
}

int uart_data_test(int argc,char *argv[])
{
	int8u addr;
	int8u data;

	if(argc > 2){
		addr = atoh(argv[1]);
		data = atoh(argv[2]);
		printf("\n\rInput 1 = %x, Input 2 = %x \n\r",addr,data);

	}
	else{
		printf("\n\r INPUT ERROR !!!! \n\r");
		printf(" Ex) tst ### ### \n\r");

	}
	return(1);
}

int get_nstring(char buf[])
{
	int i;
	int8u c;
	char rxchar[1];
	int8u tbuf[2];

	tbuf[1] = '\0';

	for (i=0 ; i<20 ; i++)
	{
		buf[i] = '\0';
	}
	for(i=0; i <20;){

		while(get_char_os(rxchar)==0);
		c=rxchar[0];
			if(( c == '\n')||(c == '\r'))
			{
				break;
			}
			if(c== 0x8)
			{
				if(i>0)
				{
					tbuf[0] = 0x8; printf((char*)tbuf);
					tbuf[0] = ' '; printf((char*)tbuf);
					tbuf[0] = 0x8; printf((char*)tbuf);
					i--; buf[i] = '\0';
				}
			}
			else{
					tbuf[0] = c; printf((char*)tbuf);
					buf[i] = c;
					i++;
			}
	}
	for( i=0; i <20; i++){
		if(buf[i] == '\0'){
			return(i);
		}
		buf[i] = ktolower(buf[i]);
	}
	return(i);
}

char get_io_number(unsigned int *retval)
{
	byte c;
	char tmp[20];
	char flag;
	int no,i;

	*retval = 0;
	flag = 0;
	no = 0;

	if(get_nstring(tmp)!=0){
		for(i=0; i<32 ; i++){
			c = tmp[i];
			if(c == '\0'){
				flag = '\n';
				break;
			}
			if((c=='/')||(c =='.')||(c==']')||(c==0x1a)||(c=='y')||(c=='n'))
			{
				flag =c;
				break;
			}
			if((c>='0')&&(c <='9'))
			{
				*retval = *retval *0x10 + c -'0';
				no++;
			}
			else
			{
				flag = ' ';
				break;

			}
		}
	}
	if(no == 0)
	{
		*retval = 0xffffffff;
	}
	return(flag);

}

void cmdcpy(char *s1, char *s2)
{
	while(1)
	{
		if(*s2 == 0)
		{
			break;
		}
		*s1++ = *s2++;
	}
}

int fill(int8u *sp, int8u c, int n)
{
	while(--n>=0)
		{
			*sp++ = c;
		}
		return(YES);
}

int kstrlen(char *s)
{
	register int cnt;

	cnt = 0;
	while(*s++)
	{
		cnt++;
	}
	return(cnt);
}

int parse_string(unsigned char *s,char *argv[])
{
	struct{
		int start;
		int end;
	}arg_tok[32];

	unsigned char *cptr;
	register int i;
	int argc = 0;

	cptr = s;

	for(i = 0, cptr = s; i< kstrlen((char*)s);i++,cptr++)
	{
		if(*cptr !=' '&& *cptr !='\t')
		{
			if(i==0 || *(cptr-1)==' '|| *(cptr-1) == '\t')
			{
				arg_tok[argc].start = i;

			}

			if(*(cptr+1)==' '||i == (kstrlen((char*)s)-1)||*(cptr+1) == '\t')
			{
				arg_tok[argc].end = i;
				argc++;
			}
		}
	}

	if(!argv)
		return(argc);

	for(i = 0; i<argc ; i++)
	{
		argv[i] = (char*)s + arg_tok[i].start;
		*(s+arg_tok[i].end+1) = 0;
	}
	return(argc);
}


char tbuf[5],omon_buf[80];
int  onch_no = 0;

void onl_monitor(unsigned char c)
{
	Cmd_tbl *cmd_tblp;
	int i,argcl;
	char *argvl[20] = {0};
	char tbuf[5];

	tbuf[1] = '\0';
	if(c == '\r'|| c =='\a')
	{
		if(onch_no == 0)
		{
			printf("\n\rARM> ");
		}
		else
		{
			for(i=0; i<20; i++)
			{
				if(omon_buf[i] == '\0')
				{
					break;
				}
				omon_buf[i] = ktolower(omon_buf[i]);
			}

			argcl = parse_string((int8u*)omon_buf,argvl);
			cmd_tblp = (Cmd_tbl*)cmd_ctbl;

			while(cmd_tblp->str)
			{
				if(!kstrncmp(argvl[0],cmd_tblp->str,10))
				{
					printf("\n\r");
					(cmd_tblp->func)(argcl,argvl);
					if(cmd_tblp->func == bfm_msg_mon)
					{
						printf("\r\nBF loading...");
					}
					else
					{
						printf("\n\rARM> ");
					}
					break;
				}
				cmd_tblp++;
			}
			if(!cmd_tblp->str)
			{
				printf("\n\rARM> Command not found !!\n\r");
			}
			fill((int8u*)omon_buf,0,80);
			onch_no = 0;
		}


	}



	else{
		if(c==0x8){
			if(onch_no>0){
				tbuf[0] = 0x8;
				tbuf[1] = ' ';
				tbuf[2] = 0x8;
				tbuf[3] = '\0';
				printf((char*)tbuf);
				onch_no--;
				omon_buf[onch_no]='\0';

			}
		}
		else{
			tbuf[0] = c;
			tbuf[1] = 0;
			printf((char*)tbuf);
			omon_buf[onch_no] = c;
			onch_no++;
		}
	}
}

int monitor_chelp(int argc,char *argv[])
{
	Cmd_tbl *cmd_tblp;
	char help_buf[80];

	printf("      [COMMAND HELP LIST]\n\r");
	printf("----------------------------------------------\n\r");
	cmd_tblp = (Cmd_tbl*)cmd_ctbl;

	while(cmd_tblp->str){
		strcpy(help_buf,"       :                            \n\r");
		cmdcpy((char*)&help_buf[1],(char*)cmd_tblp->str);
		cmdcpy((char*)&help_buf[17],(char*)cmd_tblp->useage);
		printf((char*)help_buf);
		cmd_tblp++;
	}
	printf((char*)"--------------------------------------------\n\r");
	return(YES);
}

int monitor_lshelp(int argc, char *argv[])
{
	Cmd_tbl *cmd_tblp;
	char help_buf[80], i = 0;

	printf("       [MONITOR COMMAND LIST]\n\r");
	printf("----------------------------------------\n\r");
	cmd_tblp = (Cmd_tbl*)cmd_ctbl;

	while(cmd_tblp->str){
		strcpy(help_buf,"        ");
		cmdcpy((char*)&help_buf[1],(char*)cmd_tblp->str);
		printf((char*)help_buf);
		i++,cmd_tblp++;
		if((i%0x5)==0){
			printf("\r\n");
		}
	}
	printf((char*)"\r\n--------------------------------------------\n\r");
	return(YES);
}

void prt_hexbnb(int n, int8u *hex)
{
	byte buf[50],c,i;

	i = 0;

	while(n--)
	{
		c = *hex / 16;
		if(c > 9)
		{
			c +=7;
		}
		buf[i++] = c + '0';
		c = (*hex & 0xf);
		if(c>9)
		{
			c+=7;
		}
		buf[i++] = c+'0';
		buf[i++]=' ';
		hex++;
	}
	buf[i++] = ' ';
	buf[i] = '\0';
	printf((char*)buf);
}

void os_delay(int delay_value)
{
	portTickType xTestRate;

	xTestRate = (portTickType) delay_value;
	vTaskDelay(xTestRate);
}

const Cmd_tbl cmd_ctbl[] = {
		{"help",          monitor_chelp,"monitor help"},
		{"ls",  monitor_lshelp,"Command List"},
		{"dm", dsp_mem, "Display Memory"},
		{"cm", chg_mem, "Change Memory"}
};

int dsp_mem(int argc,char*argv[])
{
	int8u tbuf[17];
	int8u *ST,*AT;
	char flag;
	int i,j;
	unsigned int start;
	flag = 1;
	if(argc==1)
	{
		printf("address error!\r\n");
		flag = 0;
	}
	if(flag == 1)
	{
		start = atoh(argv[1]);
		if(start >0xffffffff)
		{
			printf("address error!!!!!\r\n");
			return(1);
		}
		tbuf[16] = '\0';
		printf("\r\n\n");
		printf("Address 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
		printf("====== ================== ==================== \r");
		AT = ST = (int8u*)start;
		for( j =0 ; j <16 ; j++)
		{
			printf("\r\n%08x ",start);
			prt_hexbnb(8,ST);
			printf("- ");
			ST+=8;
			prt_hexbnb(8,ST);
			ST+=8;
			for(i=0;i<16;i++)
			{
				if(*AT>=0x20 && *AT<0x7f)
				{
					tbuf[i] = *AT;
				}
				else
				{
					tbuf[i] = '.';
				}
				AT++;
			}
			printf(" |");
			printf((char*)tbuf);
			start +=16;

		}
	}
	printf("\r\n\n*");
	return(1);
}

int chg_mem(int argc,char *argv[])
{
	int8u *ST;
	char flag,ret;
	u32 start;
	unsigned int data;

	flag = 1;

	if(argc == 1){
		printf("Address: ");
		start = atoh(argv[1]);
		ret = get_io_number(&start);
		if((ret !='\n')||(start == 0xffffffff))
		{
			printf("address error!\r\n");
			flag = 0;
		}
	}
	else{
		start = atoh(argv[1]);
		if(!((start>=0x20010000)&&(start<=0x200bffff)))
		{
			printf("address error!\r\n");
			return YES;
		}
	}
	if(flag ==1)
	{
		ST = (int8u*)start;
		printf("\r");
		while(1)
		{
			printf("\r\n%08x %02x - ",start,*ST);
			ret = get_io_number(&data);
			if(ret == '/')
			{
				printf("\r\n\n");
				break;
			}
			else if((ret==']')||(ret==0x1a))
			{
				start--;
				ST--;
				continue;
			}
			else if(ret=='.')
			{
				continue;
			}
			else if(ret == 0)
			{
				start++;
				ST++;
			}
			else if(ret == '\n')
			{
				*ST = data;
				start++;
				ST++;
			}
			else if(data > 0xff)
			{
				printf("\r\nDATA Input error!\r\n");
			}
			else
			{
				start++;
				ST++;
			}
		}

	}
	return(1);
}

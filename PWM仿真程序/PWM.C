#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
#define MAIN_Fosc   11059200UL //宏定义时钟HZ
//sfr P4=0xe8;
//sbit DAC_data=P4^4;
sbit DAC_data=P1;
uchar DAC_VAL;//占空比参数
uchar pwm_t;//周期
bit flag;

void delay(uint ms)
{
	uint i;
	do{
		i=MAIN_Fosc/96000;
			while(--i);
		}while(--ms);
}
void time0Init()
{
	TMOD=0x02;
	TH0=220;
	TL0=220;
	TR0=1;
	ET0=1;
}
void main()
{
	EA=1;
	time0Init();
	DAC_VAL=255;  //八位DA，满量程为5V
	
	while(1)
	{
		delay(5);
		if(DAC_VAL==255)
			flag=1;			 
			else if(DAC_VAL==80)
				flag=0;
		if((DAC_VAL<255)&&(flag==0))
			DAC_VAL++;
			else if((DAC_VAL>0)&&(flag==1))
				DAC_VAL--;
	}
}
void time() interrupt 1
{
	TH0=220;
	TL0=220; //11.0592MHZ晶振下占空比最大为256；输出100HZ（36*256*1.085=9999.36us；f=1/T）：；满255溢出，pwm加一
	pwm_t++; //周期加到255溢出
	if(pwm_t<=DAC_VAL)
		DAC_data=1;
	else 
		DAC_data=0;
}
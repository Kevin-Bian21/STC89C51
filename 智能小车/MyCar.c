#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char

sbit led1=P0^0;	//前车灯
sbit led2=P0^1;
sbit buzzer=P0^2;
sbit bat=P0^5;
sbit EN1=P2^0;
sbit IN1=P2^1;
sbit IN2=P2^2;
sbit led3=P1^0;

sbit EN2=P2^7;
sbit IN3=P2^6;
sbit IN4=P2^5;
uchar timer;
bit flag;

void delay(uint z)
{
	uint y;
	for(;z>0;z--)
		for(y=114;y>0;y--);
}

void UART_Init()	  //串口初始化
{	
	SCON=0x50;
	TMOD|=0x20;
	PCON=0;
	TH1=0xfd;	  //波特率为9600
	TL1=0xfd;
	TR1=1;
	ES=1;
}

void time_Init()
{
	TMOD|=0x02;
	TH0=210;
	TL0=210;
	TR0=1;
	ET0=1;
}
void go()
{
	IN1=0;
	IN2=1;
	EN1=1;
	IN3=0;
	IN4=1;
	EN2=1;
	led1=1;
	led2=0;
	flag=0;
}
void turn_back()
{
	IN1=1;
	IN2=0;
	EN1=1;
	IN3=1;
	IN4=0;	  
	EN2=1;
	led2=1;
	led1=0;
	flag=0;
}	
void stop()
{
	IN1=0;
	IN2=0;
	EN1=1;
	IN3=0;
	IN4=0;
	EN2=1;
	led1=0;
	led2=0;
	flag=0;
}	
void turn_left()
{
	IN1=0;
	IN2=1;
	EN1=1;
	IN3=0;
	IN4=1;
	EN2=1;
	if(timer<=180){
		EN1=0;
	}
	else{
		EN1=1;
	}
	flag=0;
}
void turn_right()
{
	IN1=0;
	IN2=1;
	EN1=1;
	IN3=0;
	IN4=1;
	EN2=1;
	if(timer<=180){
		EN2=0;
	}
	else{
		EN2=1;
	}
	flag=0;
}
void turn_circle()
{
	IN1=1;
	IN2=0;
	EN1=1;
	IN3=0;
	IN4=1;
	EN2=1;
	flag=0;	
}
void avoid_obstacle()
{
		if(bat==0)
		{
			stop();
			delay(500);
			IN1=0;
			IN2=1;
//			EN1=1;
			IN3=0;
			IN4=1;
//			EN2=0;
			if(timer<=80){
				EN1=0;
			}
			else{
				EN1=1;
			}
		}
		else
		{
			IN1=0;
			IN2=1;
//			EN1=1;
			IN3=0;
			IN4=1;
//			EN2=1;
			if(timer<=80){
				EN1=0;
				EN2=0;
			}
			else{
				EN1=1;
				EN2=1;
			}
		}
	}
		
	
void main()
{
	EN1=0;
	EN2=0;	
	EA=1;
	
	UART_Init();
	time_Init();
	while(1)
	{  	
		if(flag==1)
			avoid_obstacle();	
	}
}
void time() interrupt 1
{
	TH0=210;	//溢出一次1ms
	TL0=210;
	timer++;
	if(timer==200){
		timer=0;
	}
	
}
void UART() interrupt 4	   //串口中断函数
{
	if(RI)
	{			  
		RI=0;
		switch(SBUF)
		{
			case 0x01:go();break;
			case 0x02:stop();break;
			case 0x03:turn_left();break;
			case 0x04:turn_right();break;
			case 0x07:turn_circle();break;
			case 0x08:turn_back();break;
			case 0x09:flag=1;break;
			case 0x05:led3=1;break;
			case 0x06:led3=0;break;
			default:break;
		}
	}
}
/*

sbit b=P2^3;
void s()
{
	if(bat==0)
		b=0;
	else
		b=1;
}
void main()
{
	while(1){
	s();
	}
}
*/

	


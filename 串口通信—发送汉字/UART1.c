#include<reg52.h>
#include<stdio.h>
#define uint unsigned int 
#define uchar unsigned char
void delay(uint z)
{
	uint y;
	for(;z>0;z--)
		for(y=114;y>0;y--);
}
void UARTInit()
{	
	TR1=1;
	TMOD|=0x20;
	TH1=0xfd;
	TL1=0xfd;
	SCON=0x40;
}
void main()
{
	UARTInit();
	while(1)
	{
		TI=1;
		printf("单片机学习之路\n");
		while(!TI);//等待发送完
		TI=0;
		delay(1000);
	}
}
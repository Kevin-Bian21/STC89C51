#include <reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
uint bai,shi,ge,a=0;
sbit W_LE=P2^7; 
sbit D_LE=P2^6;
sbit LED=P1^7;
unsigned code Wei[10]={  0xfe,0xfd,0xfb,0xf7, 0xef,0xdf,0xbf,0x7f };
unsigned code Duan[10]={ 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,};
void delay(uint z)
{
	uchar y;
	for(;z>0;z--)
		for(y=120;y>0;y--);
}
void display(uint bat)
{
	bai=bat/100;
	shi=bat%100/10;
	ge=bat%10;	

	P0=0xff;
	W_LE=1;
	P0=Wei[7];
	W_LE=0;
	D_LE=1;
	P0=Duan[ge];
	D_LE=0;
	delay(10);

	P0=0xff;
	W_LE=1;
	P0=Wei[6];
	W_LE=0;
	D_LE=1;
	P0=Duan[shi];
	D_LE=0;
	delay(10);

	P0=0xff;
	W_LE=1;
	P0=Wei[5];
	W_LE=0;
	D_LE=1;
	P0=Duan[bai];
	D_LE=0;
	delay(10);		  
}
void time1Init()  //定时器1定时模式
{
	TMOD |= 0x10;
	TR1=1;
	TH1=0x4c;
	TL1=0x00;
}
void countInit() //定时器0计数模式
{
	TMOD |=0x05;
	TR0=1;
	TH0=0;
	TL0=0;
}

void main()
{
	time1Init();
	countInit();
	while(1)
	{	
		if(TF1==1)
		{
			TF1=0;
			TH1=0x4c;
			TL1=0x00;
			a++;
			if(a==10)
			{
				LED=~LED;  //产生脉冲，下降沿计数
				a=0;		
			}
		}	
		display(TL0);	  
	}
}


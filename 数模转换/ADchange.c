#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define MAIN_Fosc	11059200UL	//宏定义主时钟HZ
#define AD_CH0 0x94
#define AD_CH1 0xd4
#define AD_CH2 0xa4
#define AD_CH3 0xe4

sbit W_LE=P2^7;
sbit D_LE=P2^6;
sbit CS = P3^7;
sbit DCLK=P2^1;
sbit DIN=P2^0;
sbit DOUT = P2^5;

uint num;
uchar code Wei[]={0xfe,0xfb,0xfd,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code Duan[]={0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F, 0x6F};

void Delay_Ms(uint ms)
{
	 uint i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}

void display(uint j)	
{
	static uint i=4;
	P0=0xff;
	W_LE=1;
	P0=Wei[i];
	W_LE=0;
	switch(i)
	{
		case 4: D_LE=1;P0=Duan[j/1000];D_LE=0;break;
		case 5: D_LE=1;P0=Duan[j%1000/100];D_LE=0;break;
   		case 6: D_LE=1;P0=Duan[j%100/10];D_LE=0;break;
		case 7: D_LE=1;P0=Duan[j%10];D_LE=0;break;
	 }
	i++;
	if(i==8)
		i=4;
}
void Write_Byte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		DCLK=0;
		if(dat & 0x80)
			DIN=1;
		else 
			DIN=0;
		DCLK=1;
		dat<<=1;
	}
}
uint Read_Byte()
{
	uint i;
	uint dat;
	for(i=0;i<12;i++)
	{
		dat<<=1;
		DCLK=1;
		DCLK=0;
		if(DOUT)
			dat|=0x01;
	}
	return(dat);
}
uint Read(uchar Ctrl)
{
	uint dat;
	CS=0;
	Write_Byte(Ctrl);
	DCLK=0;
	_nop_();  //延时5us，进行输入输出切换
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	dat=Read_Byte();
	CS=1;
	return(dat);
}

void main()
{  
	uchar i; 
	while(1)
	{	
		if(i>=100)
		{
			num=Read(AD_CH1); //选取通道口，0是光敏，1是热敏	 
			num=num*1.2207;    //(5V/4096)
			i=0;  
		}
		display(num); 
		Delay_Ms(5);
		i++; 
	}  
}	




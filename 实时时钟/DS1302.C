#include<reg52.h>
#define uint unsigned int 
#define uchar unsigned char
sbit SCLK=P1^0;
sbit IO=P1^1;
sbit CE=P1^2;
sbit W_LE=P2^7;
sbit D_LE=P2^6;
uchar Hour,Min,Sec;
unsigned char code Duan[11] =
 {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X40};//共阴数码管显示段码值 0~9再加0X40
unsigned char code Wei[8] =
 {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; //数码管的位码,共八位，低电平有效。
void delay(uint z)
{
	uint y;
	for(;z>0;z--)
		for(y=114;y>0;y--);
}
void display(uchar Hour,Min,Sec)
{
	static uchar i;
	P0=0xff;	
	W_LE=1;
	P0=Wei[i];
	W_LE=0;
	switch(i)
	{
		case 0:D_LE=1;P0=Duan[Hour/10];D_LE=0;break;
		case 1:D_LE=1;P0=Duan[Hour%10];D_LE=0;break;
		case 2:D_LE=1;P0=Duan[10];D_LE=0;break;
		case 3:D_LE=1;P0=Duan[Min/10];D_LE=0;break;
		case 4:D_LE=1;P0=Duan[Min%10];D_LE=0;break;
		case 5:D_LE=1;P0=Duan[10];D_LE=0;break;
		case 6:D_LE=1;P0=Duan[Sec/10];D_LE=0;break;
		case 7:D_LE=1;P0=Duan[Sec%10];D_LE=0;break;
	}
	i++;
	if(i==8)
		i=0;
	delay(2);	
}
void Write(uchar cmd,dat)
{
	uchar i;
	CE=0;
	CE=1;
	for(i=0;i<8;i++)
	{
		SCLK=0;
		if(cmd&0x01)
			IO=1;
		else 
			IO=0;
		SCLK=1;
		cmd>>=1;
	}
	for(i=0;i<8;i++)
	{
		SCLK=0;
		if(dat&0x01)
			IO=1;
		else 
			IO=0;
		SCLK=1;
		dat>>=1;
	}
	CE=0;
}

uchar Read(uchar cmd)
{
	uchar i,dat;
	CE=0;
	CE=1;
	for(i=0;i<8;i++)
	{
		SCLK=0;
		if(cmd&0x01)
			IO=1;
		else 
			IO=0;
		SCLK=1;
		cmd>>=1;
	}
	for(i=0;i<8;i++)
	{	
		SCLK=0;
		dat>>=1;  
		if(IO)
			dat|=0x80; 
		SCLK=1;
	}
	CE=0;
	return(dat);		
}
uchar DATA_BCD(uchar dat)
{
	uchar dat1,dat2;
	dat2=dat;
	dat1=dat2/10;
	dat2=dat2%10;
	dat2=dat2+dat1*16;
	return(dat2);	
}
uchar BCD_DATA(uchar dat)
{
	uchar dat1,dat2;
	dat2=dat;
	dat1=dat2/16;
	dat2=dat2%16;
	dat2=dat2+dat1*10;
	return(dat2);	
}
void main()
{
	uchar i;
	Write(0x8e,0);
	Write(0x80,DATA_BCD(33));
	Write(0x82,DATA_BCD(22));
	Write(0x84,DATA_BCD(0x0d));
	Write(0x8e,0x80);
	while(1)
	{
		Write(0x8e,0);
		Sec=BCD_DATA(Read(0x81));
		Min=BCD_DATA(Read(0x83));
		Hour=BCD_DATA(Read(0x85));
		Write(0x8e,0x80);
		for(i=0;i<50;i++)
			display(Hour,Min,Sec);
	}
}



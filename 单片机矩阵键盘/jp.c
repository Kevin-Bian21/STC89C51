#include<reg52.h>
#include<intrins.h>
#define uint unsigned int 
#define uchar unsigned char
sbit W_LE=P2^7;
sbit D_LE=P2^6;
uchar num,shi,ge;
unsigned char code Duan[11] =
 {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X40};//共阴数码管显示段码值 0~9再加0X40
unsigned char code Wei[8] =
 {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; //数码管的位码,共八位，低电平有效。

void delay(uint z)
{
	uint i,y;
	for(i=z;i>0;i--)
		for(y=120;y>0;y--);
}
void key()
{	
	P3=0x0f; //行扫描
	if(P3!=0x0f)
	{
		delay(10);
		if(P3!=0x0f)
		{
			switch(P3)
			{
				case 0x0e: num=1; break;
				case 0x0d: num=5; break;
				case 0x0b: num=9; break;
				case 0x07: num=13;break;
		
			}
			P3=0xf0;	//列扫描
			switch(P3)
			{
				case 0xe0: num=num;   break;
				case 0xd0: num=num+1; break;
				case 0xb0: num=num+2; break;
				case 0x70: num=num+3; break;
			}
			while(P3!=0xf0);//松手检测，按键按下时一直满足while循环，当松开按键时，程序才能退出while循环
		} 
	}
	P3=0xff;
	if(P3!=0xff){
	delay(10);
		if(P3!=0xff){
			switch (P3){
			case 0xfe: num=17; break;
			case 0xfd: num=18; break;
			case 0xfb: num=19; break;
			case 0xf7: num=20; break;			
			}
			while(P3!=0xff);
		}
	}
}  	


	
void display()
{
	shi=num/10;
	ge=num%10;
	P0=0xff;
	W_LE=1;
	P0=Wei[0];
	W_LE=0;

	D_LE=1;
	P0=Duan[shi];
	D_LE=0;
	delay(5);

	P0=0xff;
	W_LE=1;
	P0=Wei[1];
	W_LE=0;

	D_LE=1;
	P0=Duan[ge];
	D_LE=0;
	delay(5);
}
void main()
{
	while(1)
	{
		key();
		display();
	}
}
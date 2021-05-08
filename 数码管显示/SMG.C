#include<reg52.h>
#include<intrins.h>
#define uint unsigned int 
#define uchar unsigned char
sbit W_LE = P2^7;
sbit D_LE = P2^6; 

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

//void display()//数码管静态显示
/*
{
	W_LE=1;
	P0=Wei[0];
	W_LE=0;

	D_LE=1;		   
	P0=Duan[0];
	D_LE=0;
}
void main()
{
	display();	
	while(1)
	{
	  P0=0xff; //数码管被锁存，此时改变P0口的值，数码管显示不发生变化
	}
}	
*/

	
//void display()   //每个数码管动态循环显示0~9
/*
{
	uint i,j;
	for(i=0;i<=7;i++)
	 	for(j=0;j<=9;j++)
		{  
			W_LE=1;
			P0=Wei[i];
			W_LE=0;

			D_LE=1;
			P0=Duan[j];
			D_LE=0;
			delay(500);  //延时半秒
		}
}
void main()
{			   	
	while(1)
	{
		display();
	}
}

*/
	
//void display()  //8个数码管动态显示0~9
/*
{
	uchar i;
	for(i=0;i<=7;i++)
	{
		P0=0xff;
		W_LE=1;
		P0=Wei[i];
		W_LE=0;

		D_LE=1;
		P0=Duan[i];
		D_LE=0;
		delay(2);  //延时
	}		
}
void main()
{
	while(1)
	{
		display();
	}
}	
*/
	
void display(uint k)   //动态显示自己设定想要显示的数字
{
	uint bai,shi,ge;
	bai=k/100;
	shi=k%100/10;
	ge =k%10;
	
	P0=0xff;
	W_LE=1;
	P0=Wei[0];
	W_LE=0;

	D_LE=1;
	P0=Duan[bai];
	D_LE=0;
	delay(5);
	
	P0=0xff;
	W_LE=1;
	P0=Wei[1];
	W_LE=0;

	D_LE=1;
	P0=Duan[shi];
	D_LE=0;
	delay(5);

	P0=0xff;	
	W_LE=1;
	P0=Wei[2];
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
		display(123);
	}
}	
	
	
	
	
	
	
	
	
	
		
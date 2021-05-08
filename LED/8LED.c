#include"reg51.h"
#include<intrins.h>
#define uint unsigned int
sbit beep=P2^3;

void delay(uint z)
{	unsigned i,y;
	for(i=z;z>0;z--)  
		for(y=120;y>0;y--);
}
/*
void main()
{
	unsigned i;
	while(1)
	{
		P1=0x80;
		delay();	
		for(i=0;i<8;i++)
		{	
			P1=P1>>1;
			beep=~beep;
			delay(); 
		}  
		
	}
}
*/



void main()
{	
	unsigned a;
	P1=0xfe;	
	while(1)
	{ 
	 	a=P1;
		P1=_cror_(a,1);
		beep=0;   	
		delay(500);
		beep=1;
		delay(500); 
	}
}							
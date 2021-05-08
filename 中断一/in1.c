#include <reg52.h>
sbit Key_s2 = P3^0;
sbit flag = P3^7;

void delay(unsigned int z)
{	
	unsigned int  y ;
	for(;z>0;z--)
		for(y=120;y>0;y--);
}
		
void int1() interrupt 2
{
	P1=~P1;
}
void main()
{
	EA=1;//开总中断
	EX1=1;//允许外部中断1中断
	IT1=1;//下降沿触发
	while(1)
	{		
		if(Key_s2==0)
			{
				delay(10);
				if(Key_s2==0)
				{	
					flag=1;
					flag=0;
				}
				while(!Key_s2);	//松手检测
			}
	}
}
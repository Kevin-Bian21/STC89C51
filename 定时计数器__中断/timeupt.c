#include<reg52.h>
#include<intrins.h>
#define uint unsigned int 
#define uchar unsigned char
sbit W_LE=P2^7;
sbit D_LE=P2^6;
sbit Key_s2=P3^0;
uchar num;
uchar code Wei[]={0xfe,0xfb,0xfd,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code Duan[]={0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F, 0x6F,};
void delay(uchar z)
{
	uchar y;
	for(;z>0;z--)
		for(y=114;y>0;y--);
}

void display(uchar j)
{
	static uchar i=5;
	P0=0xff;
	W_LE=1;
	P0=Wei[i];
	W_LE=0;
	switch(i)
	{
		case 5: D_LE=1;P0=Duan[j/100];D_LE=0;break;
   		case 6: D_LE=1;P0=Duan[j%100/10];D_LE=0;break;
		case 7: D_LE=1;P0=Duan[j%10];D_LE=0;break;
	 }
	i++;
	if(i>7)
		i=5;
}
	  
void time0Init()
{
	TMOD=0x01;
	TR0=1;
	TH0=0xee;
	TL0=0x00;
}	                
void main ()
{	
	EA=1;  //开总中断
	ET0=1; //允许To中断
	time0Init();
	while(1)
	{
		if(Key_s2==0)
		{
			delay(20); //按键消抖
			if(Key_s2==0)
			{
			   num++;
			   if(num>101)
			   		num=0;
				while(!Key_s2);//松手检测
			} 
		}			 
	}  
}
void time0() interrupt 1//定时器0溢出中断
{
	TH0=0xee;
	TL0=0x00; //定时五毫秒
	display(num);
}	                 
                  
                  
                  
                  
                 
                  
                 
     
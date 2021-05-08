#include <reg52.h>
#define uint unsigned int 
#define uchar unsigned char

uchar IRtime;	  //储存检测红外高低电平持续时间
uchar IRcode[4];  //储存解码后的4个字节数据
uchar IRdata[33]; //包含起始码在内的33位数据
bit IRjm_ok;	  //解码后4个字节数据接收完成标志位
bit IRok;		  //33位数据接收完成标志
void delay(uint z)
{
	uint y;
	for(;z>0;z--)
	for(y=114;y>0;y--);		
}
void time0Init()
{
	TMOD|=0x02;
	TH0=TL0=0;
	EA=1;
	ET0=1; //允许定时器0溢出中断
	TR0=1; //启动定时器0
	IT0=1; //下降沿触发
	EX0=1; //允许外部中断0中断

}
void UARTInit()	 //串口波特率初始化
{
	TMOD|=0x20;
	TH1=TL1=0xfd;
	SCON=0x40;	//SM1=1；
	TR1=1; //启动定时器1
}
void IRJieMa()
{
	uchar i;//i是用于计数处理4个字节
	uchar j;//j用于计数处理1个字节的8位数据
	uchar k;//k用于计数处理33次脉宽
	k = 1;//从第一位脉宽开始处理，丢掉起始码
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			IRcode[i]>>=1;
			if(IRdata[k]>6)	  //通过时间判断是0是1
				IRcode[i]|=0x80;
			k++;
		}
	}
	IRjm_ok=1;
}
				
void main()
{
	uchar i;
	time0Init();
	UARTInit();
	while(1)
	{
		if(IRok)	  //判断33位数据是否接受完成
		{
			IRJieMa();
			IRok=0;
			if(IRjm_ok)	  //判断解码是否完成
			{	
				for(i=0;i<4;i++)
				{
					SBUF=IRcode[i];
					delay(1);	 //延时一毫秒使数据能够发完 
					if(TI==1)
					TI=0;
				}
				IRjm_ok=0;
			}
		}
	}
}


void time0() interrupt 1	 //定时器0溢出中断
{
	TH0=TL0=0;				 //每中断一次所需时间为256*1.085=277.76us
	IRtime++;
}

//接收数据，存入33次脉宽
void time1() interrupt 0	 //外部中断0中断
{
	static uchar k;
	static bit startflag;
	if(startflag)
	{
		if((IRtime>40)&&(IRtime<55))  //判断起始码的时间在13.5ms左右
			k=0;
		IRdata[k]=IRtime;
		IRtime=0; //计算脉冲时间清0
		k++;
		if(k==33)
		{
			k=0;
			IRok=1;
		}
	}
	else
	{
		IRtime=0;
		startflag=1;
	}
}

				










#include<reg52.h>
#include<intrins.h>
#define uint unsigned int 
#define uchar unsigned char
sbit W_LE=P2^7;
sbit D_LE=P2^6;
uchar num;
//sbit Key_s2=P3^0;

uchar code Wei[]={0xfe,0xfb,0xfd,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code Duan[]={0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F, 0x6F,};

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
//定时器0初始化	  
void time0Init()
{
	TMOD|=0x01;
	TR0=1;
	TH0=0xee;
	TL0=0x00;
} 
//串口初始化
void UARTInit() 
{
	SM0=0; SM1=1;// 或SCON=0x50(包含了REN）;设置串行口工作方式，8位UART波特率可变
	REN=1; //允许串行口接收
	PCON=0;
	TMOD|=0x20;
	TH1=0xfd;	   //比特率9600
	TL1=0xfd;
	TR1=1; //启动定时器  
}
void main ()
{	
	EA=1;  //开总中断
	ET0=1; //允许To中断
	ES=1;	//允许串行口中断
	UARTInit();
	time0Init();
 

	while(1); 
}

//定时器0中断函数
void time0() interrupt 1//定时器0溢出中断
{
	TH0=0xee;
	TL0=0x00; //定时五毫秒
	display(num);
}  
//串口中断函数	                 
void UART() interrupt 4 //串行口中断
{	
//	uchar bemp;
	if(RI)    //接收中断请求标志位，当接收到第八位时，RI会被置一；该位由软件清零
	{
		num=SBUF;
		if(SBUF==1)
		P1=0;
		else if(SBUF==2)
			P1=1;
		RI=0;

	//	bemp=num;
	//	SBUF=++bemp;
	//	SBUF=num;
	}
	if(TI) //TI=1；发送数据成功
		TI=0;
}	                 
                  
                  
                  				
                 
                  
                 
     
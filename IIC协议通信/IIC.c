#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit W_LE=P2^7;
sbit D_LE=P2^6;
sbit SCL=P2^1;
sbit SDA=P2^0;
uchar num;
bit ACKflag;
uchar code Wei[]={0xfe,0xfb,0xfd,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code Duan[]={0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F, 0x6F};
void delay5us()
{
	_nop_();_nop_();_nop_();_nop_();_nop_();
}
void delay1(uint z)
{
	uint y;
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
void time0Init()   //定时计数器0工作模式1初始化
{
	TMOD|=0x01;
	TR0=1;
	ET0=1;
	TH0=0xee;
	TL0=0x00;
}

void IICStart()	  //起始信号
{
	SCL=1;
	SDA=1;
	delay5us();
	SDA=0;
	delay5us();
}
void IICStop()	 //终止信号
{
	SCL=0;
	SDA=0;
	SCL=1;
	delay5us();
	SDA=1;
	delay5us();
}
bit ReadACK()	 //主机读应答
{
	SCL=0;
	SCL=1;
	delay5us();
	if(SDA)
	{
		SCL=0;
		return(1);
	}
	else
	{
		SCL=0;
		return(0);
	}
}
void SendACK(bit i)	  //主机发送应答
{
	SCL=0;
	if(i)
	{
		SDA=1;
	}
	else 
		SDA=0;
	SCL=1;
	delay5us();
	SCL=0;
	SDA=1;
}




//依次写入一个字节的数
void SendByte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		SCL=0;
		if(dat & 0x80)
			SDA=1;
		else 
			SDA=0;
		SCL=1;
		dat<<=1;
	}
	SCL=0;
	SDA=1;    //释放数据总线
}
//主机往从机里写数据
void Write(uchar address,dat)
{
	IICStart();
	SendByte(0xa0+0);  //发送从机地址和写方向
	if(ReadACK())  //判断从机是否应答
		ACKflag=1;
	else 
		ACKflag=0;
	SendByte(address); //写入首地址
	if(ReadACK())  //判断从机是否应答
		ACKflag=1;
	else 
		ACKflag=0;
	SendByte(dat);
	if(ReadACK())  //判断从机是否应答
		ACKflag=1;
	else 
		ACKflag=0;
	IICStop();
}





//一个字节的数依次读出
uchar ReceiveByte()	 //有返回值
{
	uchar i,dat;
	for(i=0;i<8;i++)
	{
		dat<<=1;
	 	SCL=0;
		SCL=1;
		if(SDA)
			dat|=0x01;
	}
	return(dat);
}
//主机从从机中读取
uchar Read(uchar address)	   //有返回值
{
	uchar dat;
	IICStart();
	SendByte(0xa0+0);  //发送从机地址和读写方向
	if(ReadACK())  //判断从机是否应答
		ACKflag=1;
	else 
		ACKflag=0;
   	SendByte(address);  //发送要从从机读取数据的地址
	ReadACK();
	IICStart();
	SendByte(0xa0+1);  //发送从机地址和读数据
	if(ReadACK())  //判断从机是否应答
		ACKflag=1;
	else 
		ACKflag=0;
	dat=ReceiveByte();
	SendACK(1);
	IICStop();
	return(dat);
}

void main()
{
	time0Init();
	EA=0;
	Write(3,0x03);	 //写入的地址和数据
	delay1(5);
	num=Read(2);	 //读该地址中的数据
	EA=1;
	while(1);
}

void time() interrupt 1
{	
	TH0=0xee;
	TL0=0x00;
	display(num);
}












#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit RS=P3^5; //数据/命令
sbit RW=P3^6; //读/写
sbit EN=P3^4; //使能
void Read_Busy()
{
	RS=0;
	P0=0xff;
	RW=1;
	do
	{
		EN=1;
	}while(P0&0x80);	
}
void Write_Cmd(uchar cmd)
{	
	Read_Busy();
	RS=0;
	RW=0;
	P0=cmd;
	EN=1;
	EN=0;
}
void Write_Dat(uchar dat)
{
	Read_Busy();
	RS=1;
	RW=0;
	P0=dat;
	EN=1;
	EN=0;
}
void Max(uchar a,b,uchar *max)
{
	*max=(a>b)?a:b;
}
void main()
{
	uchar c;
	Write_Cmd(0x38);//设置16*2显示
	Write_Cmd(0x0e);//开显示 ，开光标
	Write_Cmd(0x01); //清屏
	Write_Cmd(0x06);// 地址指针移位
	Write_Cmd(0x80+0X40);// 显示的地址
	Max(6,3,&c);
	Write_Dat(c+'0'); //写入数据
	while(1);
}

		 
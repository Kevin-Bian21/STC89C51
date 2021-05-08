#include "LCD1602.h"

void Read_Busy()  //读忙信号
{
	RS=0;
	RW=1;
	P0=0xff;	 
	do{
		EN=1;
	}while(P0&0x80);  //第八位为1禁止读写
	EN=0;
}
void Write_cmd(uchar cmd)
{
	Read_Busy();
	RS=0;
	RW=0;
	P0=cmd;
	EN=1;
	EN=0;
}
void Write_data(uchar dat)
{
	Read_Busy();
	RS=1;
	RW=0;
	P0=dat;
	EN=1;
	EN=0;
}
void DisplayChar(uchar x,y,dat)  //显示一个字节数据
{
	if(y)
	x|=0x40;  //y=1，显示在第二行
	x|=0x80;  //否则显示在第一行
	Write_cmd(x);
	Write_data(dat);
}
void DisplayOneStr(uchar x,y,dat)  //显示一个字节字符
{
	DisplayChar(x++,y,dat/16+'0');
	DisplayChar(x,y,dat%16+'0');
}

void DisplayStr(uchar x,y,uchar *str)
{
	uchar addr;
	if(y==0)
		addr=0x00+x;
	else
		addr=0x40+x;
	Write_cmd(addr|0x80);
	while(*str!='\0')
		Write_data(*str++);
}
void Init_1602()
{
	Write_cmd(0x38);  //选择16*2显示
	Write_cmd(0x0c);  //开显示，开光标
	Write_cmd(0x06);  //读写一个字节后地址指针加一
	Write_cmd(0x01); //清屏
}





#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit RS=P3^5;  //数据/命令选择端
sbit RW=P3^6;  //读/写数据选择端
sbit EN=P3^4;  //使能端
void Read_Busy();
void Write_cmd(uchar cmd);
void Write_data(uchar dat);
void DisplayChar(uchar x,y,dat);
void DisplayOneStr(uchar x,y,dat);
void DisplayStr(uchar x,y,uchar *str);
void Init_1602();

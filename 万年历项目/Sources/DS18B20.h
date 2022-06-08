#include <reg52.h>
#include <intrins.h>
#define uint unsigned int 
#define uchar unsigned char
sbit DS=P2^2; //DS单总线传输数据位
bit Init_DS18B20();
void Write_DS18B20(uchar dat);
uchar Read_DS18B20();
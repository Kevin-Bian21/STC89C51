#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define DS1302_W_addr 0x80
#define DS1302_R_addr 0x81
sbit SCLK=P1^0;
sbit IO=P1^1;
sbit CE=P1^2;
void Write_byte(uchar cmd,dat);
uchar Read_byte(uchar cmd);
uchar Data_BCD(uchar dat);
uchar BCD_Data(uchar dat);

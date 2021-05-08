#include "DS18B20.h"
void delay(uchar a)  
{
    while(a--);
}

bit Init_DS18B20()	  //��ʼ������DS18B20���ش���ָ��
{
	uchar i;
	DS=1;
	DS=0;
	delay(75);
	DS=1;
	delay(4);
	i=DS;
	delay(20);
	return (i);
}

void Write_DS18B20(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{  
		DS=0;
		_nop_();
		if(dat&0x01)
			DS=1;
		else
			DS=0;
		delay(10);
		DS=1;  //�ͷ����ߣ��Ա���һ��д��
		_nop_();
		dat>>=1;
	}
}
uchar Read_DS18B20()
{
	uchar i;
	uchar dat;
	for(i=0;i<8;i++)
	{
		DS=0;
		_nop_();
		DS=1;
		_nop_();
		dat>>=1;
		if(DS)
			dat|=0x80;
		delay(10);
		DS=1;
		_nop_();
	}
	return(dat);
}


				
#include "DS1302.h"
void Write_byte(uchar cmd,dat)
{
	uchar i;
	CE=0;
	CE=1;
	for(i=0;i<8;i++)
	{
		SCLK=0;
		if(cmd&0x01)
			IO=1;
		else
			IO=0;
		SCLK=1;
		cmd>>=1;
	}	
	for(i=0;i<8;i++)
	{
		SCLK=0;
		if(dat&0x01)
			IO=1;
		else
			IO=0;
		SCLK=1;
		dat>>=1;
	}
	CE=0;	
}
uchar Read_byte(uchar cmd)
{
	uchar i,dat;
	CE=0;
	CE=1;
	for(i=0;i<8;i++)
	{
		SCLK=0;
		if(cmd&0x01)
			IO=1;
		else
			IO=0;
		SCLK=1;
		cmd>>=1;
	}
	for(i=0;i<8;i++)
	{
		SCLK=1;	  //时钟总线处于高位时，1302将数据放到总线上
	  	SCLK=0;	  //拉低时钟总线，即可读出数据
		dat>>=1;
		if(IO)
			dat|=0x80;
	}
	CE=0;
	return (dat);
}
uchar Data_BCD(uchar dat)
{
	uchar dat1,dat2;
	dat2=dat;
	dat1=dat2/10;
	dat2=dat2%10;
	dat2=dat2+dat1*16;
	return(dat2);	
}

		
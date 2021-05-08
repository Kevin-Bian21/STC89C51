#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit SH_CP = P3^0;	//串行输入时钟信号
sbit ST_CP = P3^3;	//并行输出时钟信号
sbit DS = P3^1;   //串行数据输入端

//uchar code a[]={};
void main()
{
	uchar i,j;
	i=0x80;	//列，高电位有效
	for(j=0;j<8;j++)
	{
		SH_CP=0;
		ST_CP=0;
		if(i & 0x01)
			DS=1;
		else
			DS=0;
		SH_CP=1;
		i>>=1;
	}
	i=0x7f;	 //行，低电位有效
	for(j=0;j<8;j++)
	{
		SH_CP=0;
		ST_CP=0;			

		if(i & 0x01)
			DS=1;
		else
			DS=0;
		SH_CP=1;
		i>>=1;
	}
	ST_CP=1;
	while(1);	
}	









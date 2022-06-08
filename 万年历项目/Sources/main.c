#include "main.h"
uchar TimeData[7]={0, 17, 20, 29, 7, 3, 20};//时钟日历暂存数组，秒/分/时，日/月/星期/年
void WriteTimeData(uchar addr) //addr写时钟的地址命令
{
	uchar i;
	Write_byte(0x8e,0x00);  //关闭写保护
	for(i=0;i<7;i++)
	{	
		Write_byte(addr,Data_BCD(TimeData[i]));
		addr+=2;
	}
	Write_byte(0x8e,0x80);	//开写保护
}
void ReadTimeData(uchar addr)
{
	uchar i;
	Write_byte(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		TimeData[i]=Read_byte(addr);
		addr+=2;
	}
	Write_byte(0x8e,0x80);
}


 void KeyScan()
{ 
	if(S2==0)
		delay_ms(10);
		if(S2==0)
		{
			CE=0;			
			Write_cmd(0x38);  //选择16*2显示
			Write_cmd(0x0e);  //开显示，开光标,开闪烁
			
			if(S4==0)
			{
					
				Write_byte(0x8e,0x00);
				TimeData[1]=Read_byte(0x49);
				Write_byte(0x8e,0x80);
				if(S4==0)
				{
					TimeData[1]++;
				}
			}
		}
		if(S3==0)
		{
			CE=1;
			Write_byte(0x8e,0x00);  //关闭写保护	
			Write_byte(0x49,Data_BCD(TimeData[1]));
			Write_byte(0x8e,0x80);	//开写保护
			Write_byte(0x8e,0x00);
			TimeData[1]=Read_byte(0x49);
			Write_byte(0x8e,0x80);
		}  
}
void main()
{
	uint j;
	uchar H,L,shi,ge;
	Init_1602();
	WriteTimeData(DS1302_W_addr);
	DisplayStr(0,0,"Date:");
	DisplayStr(0,1,"Time:");
	Init_DS18B20();		  //每次读写之前都要进行初始化
	Write_DS18B20(0xcc);  //发送跳跃ROM指令
	Write_DS18B20(0x4e);  //写暂存器指令
	Write_DS18B20(0x1f);  //TH寄存器
	Write_DS18B20(0x0f);  //TL寄存器
	Write_DS18B20(0x7f);  //配置工作在12位模式下
/*	Init_1602();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x48);   *///将配置好的存到EEPROM中，
	while(1)
	{
		ReadTimeData(DS1302_R_addr);
		KeyScan();
		DisplayOneStr(5,0,TimeData[6]);
		DisplayChar(7,0,'-');
		DisplayOneStr (8,0,TimeData[4]); //月
		DisplayChar(10,0,'-');		 //-
		DisplayOneStr(11,0,TimeData[3]);//日
		DisplayChar(13,0,'-');		 //-
		DisplayOneStr(14,0,TimeData[5]);//星期

		DisplayOneStr(5,1,TimeData[2]); //时
		DisplayChar(7,1,':');		 //：
		DisplayOneStr(8,1,TimeData[1]); //分
		DisplayChar(10,1,':');		 //：
		DisplayOneStr(11,1,TimeData[0]);//秒

		Init_DS18B20();		  //每次读写之前都要进行初始化
		Write_DS18B20(0xcc);  //发送跳跃ROM指令
		Write_DS18B20(0x44);  //发送温度转换指令
		Init_DS18B20();
		Write_DS18B20(0xcc);
		Write_DS18B20(0xbe);  //读取DS18B20暂存器值
		L=Read_DS18B20();
		H=Read_DS18B20();
		j=H;
		j<<=8;
		j|=L;
		j=j*0.0625*10+0.5;	   //分辨率为0.0625
		shi=j/100;
		ge=j%100/10;
		DisplayChar(14,1,shi+'0');
		DisplayChar(15,1,ge+'0');
	}
}
		

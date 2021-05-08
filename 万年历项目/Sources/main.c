#include "main.h"
uchar TimeData[7]={0, 17, 20, 29, 7, 3, 20};//ʱ�������ݴ����飬��/��/ʱ����/��/����/��
void WriteTimeData(uchar addr) //addrдʱ�ӵĵ�ַ����
{
	uchar i;
	Write_byte(0x8e,0x00);  //�ر�д����
	for(i=0;i<7;i++)
	{	
		Write_byte(addr,Data_BCD(TimeData[i]));
		addr+=2;
	}
	Write_byte(0x8e,0x80);	//��д����
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
			Write_cmd(0x38);  //ѡ��16*2��ʾ
			Write_cmd(0x0e);  //����ʾ�������,����˸
			
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
			Write_byte(0x8e,0x00);  //�ر�д����	
			Write_byte(0x49,Data_BCD(TimeData[1]));
			Write_byte(0x8e,0x80);	//��д����
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
	Init_DS18B20();		  //ÿ�ζ�д֮ǰ��Ҫ���г�ʼ��
	Write_DS18B20(0xcc);  //������ԾROMָ��
	Write_DS18B20(0x4e);  //д�ݴ���ָ��
	Write_DS18B20(0x1f);  //TH�Ĵ���
	Write_DS18B20(0x0f);  //TL�Ĵ���
	Write_DS18B20(0x7f);  //���ù�����12λģʽ��
/*	Init_1602();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x48);   *///�����úõĴ浽EEPROM�У�
	while(1)
	{
		ReadTimeData(DS1302_R_addr);
		KeyScan();
		DisplayOneStr(5,0,TimeData[6]);
		DisplayChar(7,0,'-');
		DisplayOneStr (8,0,TimeData[4]); //��
		DisplayChar(10,0,'-');		 //-
		DisplayOneStr(11,0,TimeData[3]);//��
		DisplayChar(13,0,'-');		 //-
		DisplayOneStr(14,0,TimeData[5]);//����

		DisplayOneStr(5,1,TimeData[2]); //ʱ
		DisplayChar(7,1,':');		 //��
		DisplayOneStr(8,1,TimeData[1]); //��
		DisplayChar(10,1,':');		 //��
		DisplayOneStr(11,1,TimeData[0]);//��

		Init_DS18B20();		  //ÿ�ζ�д֮ǰ��Ҫ���г�ʼ��
		Write_DS18B20(0xcc);  //������ԾROMָ��
		Write_DS18B20(0x44);  //�����¶�ת��ָ��
		Init_DS18B20();
		Write_DS18B20(0xcc);
		Write_DS18B20(0xbe);  //��ȡDS18B20�ݴ���ֵ
		L=Read_DS18B20();
		H=Read_DS18B20();
		j=H;
		j<<=8;
		j|=L;
		j=j*0.0625*10+0.5;	   //�ֱ���Ϊ0.0625
		shi=j/100;
		ge=j%100/10;
		DisplayChar(14,1,shi+'0');
		DisplayChar(15,1,ge+'0');
	}
}
		

/*********************************************************************************
* ����дʱ�䡿�� 2019��5��21��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ (ֱ����)
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ�������� & QX-A51����С��
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52
* �����뻷������ Keil ��Visio4
* �������ܡ��� QX-MCS51 ��Ƭ��������DA���		   			            			    
* ��ʹ��˵������ ���AIN3ͨ��һ���Ű��߽ӵ�DOUT��
* ��ע������� 
**********************************************************************************/
#include <reg52.h>
#include <intrins.h>

sfr P4 = 0xe8;  /*C����������P4�����⹦�ܼĴ�����ַ*/
sbit DAC_DATA = P4^4;//DA���ݿ�
sbit CS = P3^7;
sbit DCLK = P2^1;
sbit DIN = 	P2^0;
sbit DOUT = P2^5;
sbit DU = P2^6;//����ܶ�ѡ
sbit WE = P2^7;//����ܶ�ѡ

#define MAIN_Fosc		11059200UL	//�궨����ʱ��HZ
#define AD_CH3 0xe4 
/*====================================
ʹ��typedef��������������ȡ����
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;
typedef unsigned char u8;

typedef unsigned int INT16U;
typedef unsigned int uint;
typedef unsigned int u16;

uchar DAC_VAL;//ռ�ձȲ��� ģ��8λDA�����ȡֵ��Χ0-255
uchar pwm_t;//����
uint voltage;//��ѹֵ

//��������ܶ�ѡ��0-9
uchar code SMGduan[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};
//�����λѡ��
uchar code SMGwei[] = {0xfe, 0xfd, 0xfb, 0xf7};

/*====================================
������	��void Delay_Ms(INT16U ms)
����	��ms��������ʱ�β�
����ֵ	����
����	��12T 51��Ƭ������Ӧ��ʱ�Ӻ��뼶��ʱ����
====================================*/
void Delay_Ms(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}

/*====================================
����	��display(uchar i)
����	��i ��ʾ����ȡֵ0-9999 
����ֵ	����
����	������ܶ�̬��ʾ����
====================================*/
void display(uint i)
{
	uchar q, b, s, g;
	static uchar wei;
	q = i / 1000;
	b = i % 1000 / 100;
	s = i % 100 / 10;
	g = i % 10;		
	P0 = 0XFF;//�������
	WE = 1;//��λѡ������
	P0 = SMGwei[wei];
	WE = 0;//����λѡ����
	P0 = 0XFF;//�������
	switch(wei)
	{
		case 0: DU = 1; P0 = SMGduan[q] | 0x80;	DU = 0; break;//0x80����С����
		case 1: DU = 1; P0 = SMGduan[b]; 	DU = 0; break;	
		case 2: DU = 1; P0 = SMGduan[s]; 	DU = 0; break;
		case 3: DU = 1; P0 = SMGduan[g]; 	DU = 0; break;		
	}
	wei++;
	if(wei == 4)
		wei = 0;
}

/*====================================
����	��SPI_Write(uchar DAT)
����	��DAT��Ҫ���͵�����
����ֵ	����
����	������һ���ֽ�����
====================================*/
void SPI_Write(uchar DAT)
{
	uchar i; 
	for(i=0; i<8; i++) //�ֱ�д8�Σ�ÿ��д1λ
	{
		DCLK = 0;//����ʱ�����ߣ�����DIN�仯
		if(DAT & 0x80)//��д�������λ
			DIN = 1;  //д1
		else
			DIN = 0;  //д0
		DCLK = 1;	  //����ʱ�ӣ��ôӻ���DIN
		DAT <<= 1;	  //Ϊ������һλ����1λ
	}
}
/*====================================
����	��ReadByte()
����	����
����ֵ	�����ض���������
����	��
====================================*/
uint SPI_Read()
{
	uchar i; 
	uint DAT;
	for(i=0; i<12; i++)//�ֱ��12�Σ�ÿ�ζ�һλ
	{
		DAT <<= 1; //��������1λ��׼������һλ
		DCLK = 1;   //����ʱ�����ߣ���ȡSDA�ϵ�����
		DCLK = 0;   //����ʱ�����ߣ�����ӻ�����SDA�仯
		if(DOUT)
			DAT |= 0X01;//Ϊ1��д1��������ִ��д1��ͨ�����Ʋ�0
	}
	return(DAT); //���ض���������
}

/*====================================
����	��PCF8591Read(uchar cmd)
����	��cmd XPT2046�����ֽ�
����ֵ	��ADת����������
����	����ָ��ͨ���������ģ����רΪ������
====================================*/
uint ReadAD(uchar cmd)
{
	uint DAT;
	CS = 0;
	SPI_Write(cmd);
	DCLK = 0;   //����ʱ������
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	DAT = SPI_Read();
	CS = 1;
	return(DAT);//���ض�������
			
}
//��ʱ��0�ж�
void timer0() interrupt 1
{
	pwm_t++;//�����Լӵ�255������Զ�����
	if(pwm_t <= DAC_VAL)// ����ռ�ձ�
		DAC_DATA = 1;
	else
		DAC_DATA = 0;			 
}
void main()
{
	uint i;
	TMOD |= 0x02;//8λ�Զ���װģ��
	TH0 = 220;
	TL0 = 220;//11.0592M������ռ�ձ�����ֵ��256,���100HZ
	TR0 = 1;//������ʱ��0
	ET0 = 1;//����ʱ��0�ж�
	EA	= 1;//���ж�����
//	DAC_VAL = 169; //���3.3V (һ�����������۱�ʾ���19.53125mV)
	DAC_VAL = 255;//���5V
	while(1)
	{
		if(i >= 500)//��ʱ1000����
		{
			i = 0;
			EA = 0;
			voltage = ReadAD(AD_CH3);	//ͨ��0����cmd��0x94  ͨ��1����cmd��0xd4  ͨ��2��λ��cmd��0xa4 ͨ��3�ⲿ����AIN3 cmd��0xe4
			voltage = voltage * 1.2207 ; //  ��5/4096��
			EA = 1;
		}
		display(voltage);
		Delay_Ms(1);
		i++;
	}
}
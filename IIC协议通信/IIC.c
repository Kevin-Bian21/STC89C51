#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit W_LE=P2^7;
sbit D_LE=P2^6;
sbit SCL=P2^1;
sbit SDA=P2^0;
uchar num;
bit ACKflag;
uchar code Wei[]={0xfe,0xfb,0xfd,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code Duan[]={0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F, 0x6F};
void delay5us()
{
	_nop_();_nop_();_nop_();_nop_();_nop_();
}
void delay1(uint z)
{
	uint y;
	for(;z>0;z--)
		for(y=114;y>0;y--);
}
void display(uchar j)
{
	static uchar i=5;
	P0=0xff;
	W_LE=1;
	P0=Wei[i];
	W_LE=0;
	switch(i)
	{
		case 5: D_LE=1;P0=Duan[j/100];D_LE=0;break;
   		case 6: D_LE=1;P0=Duan[j%100/10];D_LE=0;break;
		case 7: D_LE=1;P0=Duan[j%10];D_LE=0;break;
	 }
	i++;
	if(i>7)
		i=5;
}
void time0Init()   //��ʱ������0����ģʽ1��ʼ��
{
	TMOD|=0x01;
	TR0=1;
	ET0=1;
	TH0=0xee;
	TL0=0x00;
}

void IICStart()	  //��ʼ�ź�
{
	SCL=1;
	SDA=1;
	delay5us();
	SDA=0;
	delay5us();
}
void IICStop()	 //��ֹ�ź�
{
	SCL=0;
	SDA=0;
	SCL=1;
	delay5us();
	SDA=1;
	delay5us();
}
bit ReadACK()	 //������Ӧ��
{
	SCL=0;
	SCL=1;
	delay5us();
	if(SDA)
	{
		SCL=0;
		return(1);
	}
	else
	{
		SCL=0;
		return(0);
	}
}
void SendACK(bit i)	  //��������Ӧ��
{
	SCL=0;
	if(i)
	{
		SDA=1;
	}
	else 
		SDA=0;
	SCL=1;
	delay5us();
	SCL=0;
	SDA=1;
}




//����д��һ���ֽڵ���
void SendByte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		SCL=0;
		if(dat & 0x80)
			SDA=1;
		else 
			SDA=0;
		SCL=1;
		dat<<=1;
	}
	SCL=0;
	SDA=1;    //�ͷ���������
}
//�������ӻ���д����
void Write(uchar address,dat)
{
	IICStart();
	SendByte(0xa0+0);  //���ʹӻ���ַ��д����
	if(ReadACK())  //�жϴӻ��Ƿ�Ӧ��
		ACKflag=1;
	else 
		ACKflag=0;
	SendByte(address); //д���׵�ַ
	if(ReadACK())  //�жϴӻ��Ƿ�Ӧ��
		ACKflag=1;
	else 
		ACKflag=0;
	SendByte(dat);
	if(ReadACK())  //�жϴӻ��Ƿ�Ӧ��
		ACKflag=1;
	else 
		ACKflag=0;
	IICStop();
}





//һ���ֽڵ������ζ���
uchar ReceiveByte()	 //�з���ֵ
{
	uchar i,dat;
	for(i=0;i<8;i++)
	{
		dat<<=1;
	 	SCL=0;
		SCL=1;
		if(SDA)
			dat|=0x01;
	}
	return(dat);
}
//�����Ӵӻ��ж�ȡ
uchar Read(uchar address)	   //�з���ֵ
{
	uchar dat;
	IICStart();
	SendByte(0xa0+0);  //���ʹӻ���ַ�Ͷ�д����
	if(ReadACK())  //�жϴӻ��Ƿ�Ӧ��
		ACKflag=1;
	else 
		ACKflag=0;
   	SendByte(address);  //����Ҫ�Ӵӻ���ȡ���ݵĵ�ַ
	ReadACK();
	IICStart();
	SendByte(0xa0+1);  //���ʹӻ���ַ�Ͷ�����
	if(ReadACK())  //�жϴӻ��Ƿ�Ӧ��
		ACKflag=1;
	else 
		ACKflag=0;
	dat=ReceiveByte();
	SendACK(1);
	IICStop();
	return(dat);
}

void main()
{
	time0Init();
	EA=0;
	Write(3,0x03);	 //д��ĵ�ַ������
	delay1(5);
	num=Read(2);	 //���õ�ַ�е�����
	EA=1;
	while(1);
}

void time() interrupt 1
{	
	TH0=0xee;
	TL0=0x00;
	display(num);
}












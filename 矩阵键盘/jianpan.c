#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
#define WX P2
#define DX P0
 uchar num,gewei,shiwei;
 uchar code Duan[] =
 {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X40};//�����������ʾ����ֵ 0~9�ټ�0X40
 uchar code Wei[] =
 {0x0e,0x0d}; //����ܵ�λ��,����λ���͵�ƽ��Ч��

 void delay(uint z)
 {	
 	uint i,y;
 	for(i=z;i>0;i--)
		for(y=120;y>0;y--);
}

 void key_scan()
 {
 	P1=0x0f;//��ɨ��
	if(P1!=0x0f) //����������
	{
		delay(10);
		if(P1!=0x0f)
		{
			switch(P1)
			{
				case 0x0e:	num=1;break;
				case 0x0d:	num=5;break;
				case 0x0b:	num=9;break;
				case 0x07:	num=13;break;
			}
			 P1=0xf0;  //��ɨ��
			 switch (P1)
			 {
			 	case 0xe0:	num=num;break;
				case 0xd0:	num=num+1;break;
				case 0xb0:	num=num+2;break;
				case 0x70:	num=num+3;break;
			}
		//	while(P1!=0xf0);
		 }
	 }
 }
void display()
{
	shiwei=num/10;
	gewei=num%10;


	WX=Wei[0];
	DX=Duan[shiwei];
	delay(10);

	WX=Wei[1];
	DX=Duan[gewei];
	delay(10);
 }
 void main()
 {	
 	while(1)
	{
	 	key_scan();
		display();
	}
}



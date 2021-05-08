#include<reg52.h>
#include<intrins.h>
unsigned char Sec,mSec;	//秒、毫秒
sbit W_LE=P2^7; 
sbit D_LE=P2^6;
unsigned code Wei[10]={  0xfe,0xfd,0xfb,0xf7, 0xef,0xdf,0xbf,0x7f };
unsigned code Duan[10]={ 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,};
void display()
{	
	W_LE=1;
	P0=Wei[7];
	W_LE=0;

	D_LE=1;
	P0=Duan[Sec];
	D_LE=0;
}

		
void time1Init()
{
	TMOD=0x01;
	TR0=1;
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
}
void main()
{
	time1Init();
	P1=0x7f;
	while(1)
	{
		if(TF0==1)
		{
			TF0=0;
			TH0=0x3c;
			TL0=0xb0;
			mSec++;
			if(mSec==20)//1000ms之后执行下列语句
			{  	
				P1=_cror_(P1,1);
				mSec=0;
				Sec++;
				if(Sec==8)
					Sec=0;	 
			}
			display();
		}
	}
}
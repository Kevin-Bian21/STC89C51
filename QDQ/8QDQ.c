 #include <reg52.h>
 #define uchar unsigned char
 #define uint unsigned int 
 int djs1=30 ;   //����ʱ30�������ʱ��
 int djs2=40 ;   //����ʱ40�����ʱ��
 #define DX P0   //��������ܶ�ѡ�˿� 
 #define WX P2	 //����λѡ�˿�

 bit flag;       //��ʼ�����־λ
 uint num=0 , djs=0; //numѡ�ֺţ�djs����ʱ��ı���
 uchar second,count; //����second��count��������
 uchar shiwei , gewei;  //�������ʾ
 sbit Buzzer = P3^0; //����������˿�
 sbit Key1 = P1^0;
 sbit Key2 = P1^1;
 sbit Key3 = P1^2;
 sbit Key4 = P1^3;
 sbit Key5 = P1^4;
 sbit Key6 = P1^5;
 sbit Key7 = P1^6;
 sbit Key8 = P1^7;

 sbit Key10 = P3^4;
 sbit Key11 = P3^5;
 sbit Key12 = P3^6;
 sbit Key13 = P3^7;
 sbit Key14 = P2^4;
 sbit Key15 = P2^7;
  
 sbit LED1 = P3^1;
 sbit LED2 = P3^2;
 sbit LED3 = P3^3;

 unsigned char code Seg_Duan[] =
 {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X40};//�����������ʾ����ֵ 0~9�ټ�0X40
 unsigned char code Seg_Wei[] =
 {0xfe,0xfd,0xfb,0xf7}; //����ܵ�λ��,����λ���͵�ƽ��Ч��

 //������ʾ��ʱ����
 void delay( int m)
 {
 	unsigned i=0;
	for(i=0;m>i;m--);
 }
//�������𣬴��⣬����ʱ�亯��
 void Key_host ()   //�����˿���	   
{
	if(Key10==0)   //���������
	{
		TR0=1;	   //������ʱ�� 
		second=djs1;	//30�뵹��ʱ��ʼ
		flag=1;			//��־λ�� 1
		LED1=1;
		LED2=0;
		LED3=0;
		Buzzer=0;
	}
	if(Key11==0)   //���������
	{	
		TR0=1;	   //������ʱ�� 
		second=djs2;   //40�뵹��ʱ
		LED1=0;
		LED2=1;
		LED3=0;
		Buzzer=0;
	}
	if(Key12==0)	//ʱ��Ӱ���
	{
//		TR0=0;	   //ֹͣ����ʱ
		{
//		djs=djs1;
//		djs=shiwei*10+gewei;   //����ǰ��ʱ�丳��djs
		delay(300000);
		djs1 ++ ;	  //��һ�¼���djs�� 1
		second=djs1;
		}
	}		 
	if(Key13==0)
	{	
//		TR0=0;
//		djs=shiwei*10+gewei;
		delay(300000);
		djs1--;
		second=djs1;
	}
	if(Key14==0)
	{
		delay(300000);
		djs2 ++ ;	  //��һ�¼���djs�� 1
		second=djs2;
	}
	if(Key15==0)
	{
	   	delay(300000);
		djs2 -- ;	  //��һ�¼���djs�� 1
		second=djs2;
	}
}
//����ѡ�ְ�������
void Key_players()	 	   //ѡ�ֿ���
{
	if(flag==1)	       //����������º�ѡ�ֲ�������
	{
		if(Key1==0)	   //һ��ѡ�ְ��¼�λ
		{
			TR0=0;	   //�رն�ʱ��
			num=1;	   //һ��ѡ��
			flag=0;	   //flag��0��ʹ������ѡ���޷�������
		}
		if(Key2==0)
		{
			TR0=0;
			num=2;
			flag=0;
		}
		if(Key3==0)
		{
			TR0=0;
			num=3;
			flag=0;
		}
		if(Key4==0)
		{
			TR0=0;
			num=4;
			flag=0;
		}
		if(Key5==0)
		{
			TR0=0;
			num=5;
			flag=0;
		}
		if(Key6==0)
		{
			TR0=0;
			num=6;
			flag=0;
		}
		if(Key7==0)
		{
			TR0=0;
			num=7;
			flag=0;
		}
		if(Key8==0)
		{
			TR0=0;
			num=8;
			flag=0;
		}
	}
}
//�����������ʾ����
void Display()	  
{
		shiwei=second/10;     //�����ʮλ 
		gewei =second%10;     //����ܸ�λ  
	
		WX = Seg_Wei[0];      //��һ�������ѡ��
		DX = Seg_Duan[num];	  //��ʾѡ�ֺ�
		delay(300);

		WX = Seg_Wei[1];      //�ڶ��������ѡ��
		DX = Seg_Duan[10];    //��ʾһ���
		delay(300);

		WX = Seg_Wei[2];       //����λ�����ѡ��
	    DX = Seg_Duan[shiwei]; //��ʾ����ʱ��ʮλ 
	    delay(300); 
		
		WX = Seg_Wei[3];       //����λ�����ѡ�� 
		DX = Seg_Duan[gewei];  //��ʾ����ʱ�ĸ�λ 
		delay(300);
}
//����������	
void  main()
 {	 
	TMOD=0x01; //��ʱ������ 16λ  ,ʹ�ö�ʱ�� T0��ģʽ 1
	EA=1; 	  //�����ж�
	ET0=1;    //����T0�ж�
	LED1=0;
	LED2=0;
	LED3=1;
	while(1) 
	{
	    Key_host();
		Key_players();
		Display();
		if(second==0)
		{
			LED1=0;
			LED2=0;
			LED3=1;
		 }
	}	 	
}
//���嶨ʱ�жϺ���
void time(void) interrupt 1 //��ʱ��0�ж� 
{
	TH0=(65536-50000)/256; //��ʱʱ�� 50ms
	TL0=(65536-50000)%256; 
	count++; 
	if(count==20) //50���붨ʱ��50*20=1000������)=1�� 
	{
		 count=0;
		 second--; 
		if(second==1){		 //���Ʒ���������
			Buzzer=0;	
		}
		else{
			Buzzer=1;
		}
		if(second==0)//��secondΪ0ʱ��ֹͣ����ʱ
		{
			flag=0;	 
			TR0=0;			
		}	
 	}	 
}

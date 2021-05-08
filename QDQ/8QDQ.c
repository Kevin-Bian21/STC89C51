 #include <reg52.h>
 #define uchar unsigned char
 #define uint unsigned int 
 int djs1=30 ;   //倒计时30秒的抢答时间
 int djs2=40 ;   //倒计时40秒答题时间
 #define DX P0   //定义数码管段选端口 
 #define WX P2	 //定义位选端口

 bit flag;       //开始抢答标志位
 uint num=0 , djs=0; //num选手号，djs调整时间的变量
 uchar second,count; //定义second和count两个变量
 uchar shiwei , gewei;  //数码管显示
 sbit Buzzer = P3^0; //定义蜂鸣器端口
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
 {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X40};//共阴数码管显示段码值 0~9再加0X40
 unsigned char code Seg_Wei[] =
 {0xfe,0xfd,0xfb,0xf7}; //数码管的位码,共四位，低电平有效。

 //定义显示延时函数
 void delay( int m)
 {
 	unsigned i=0;
	for(i=0;m>i;m--);
 }
//定义抢答，答题，调整时间函数
 void Key_host ()   //主持人控制	   
{
	if(Key10==0)   //抢答键按下
	{
		TR0=1;	   //启动定时器 
		second=djs1;	//30秒倒计时开始
		flag=1;			//标志位置 1
		LED1=1;
		LED2=0;
		LED3=0;
		Buzzer=0;
	}
	if(Key11==0)   //答题键按下
	{	
		TR0=1;	   //启动定时器 
		second=djs2;   //40秒倒计时
		LED1=0;
		LED2=1;
		LED3=0;
		Buzzer=0;
	}
	if(Key12==0)	//时间加按下
	{
//		TR0=0;	   //停止倒计时
		{
//		djs=djs1;
//		djs=shiwei*10+gewei;   //将当前的时间赋给djs
		delay(300000);
		djs1 ++ ;	  //按一下键后djs加 1
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
		djs2 ++ ;	  //按一下键后djs加 1
		second=djs2;
	}
	if(Key15==0)
	{
	   	delay(300000);
		djs2 -- ;	  //按一下键后djs加 1
		second=djs2;
	}
}
//定义选手按键函数
void Key_players()	 	   //选手控制
{
	if(flag==1)	       //当抢答键按下后选手才能抢答
	{
		if(Key1==0)	   //一号选手按下键位
		{
			TR0=0;	   //关闭定时器
			num=1;	   //一号选手
			flag=0;	   //flag置0，使得其他选手无法再抢。
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
//定义数码管显示函数
void Display()	  
{
		shiwei=second/10;     //数码管十位 
		gewei =second%10;     //数码管个位  
	
		WX = Seg_Wei[0];      //第一个数码管选中
		DX = Seg_Duan[num];	  //显示选手号
		delay(300);

		WX = Seg_Wei[1];      //第二个数码管选中
		DX = Seg_Duan[10];    //显示一横杠
		delay(300);

		WX = Seg_Wei[2];       //第三位数码管选中
	    DX = Seg_Duan[shiwei]; //显示倒计时的十位 
	    delay(300); 
		
		WX = Seg_Wei[3];       //第四位数码管选中 
		DX = Seg_Duan[gewei];  //显示倒计时的个位 
		delay(300);
}
//定义主函数	
void  main()
 {	 
	TMOD=0x01; //定时器设置 16位  ,使用定时器 T0的模式 1
	EA=1; 	  //开总中断
	ET0=1;    //允许T0中断
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
//定义定时中断函数
void time(void) interrupt 1 //定时器0中断 
{
	TH0=(65536-50000)/256; //定时时间 50ms
	TL0=(65536-50000)%256; 
	count++; 
	if(count==20) //50毫秒定时，50*20=1000（毫秒)=1秒 
	{
		 count=0;
		 second--; 
		if(second==1){		 //控制蜂鸣器发声
			Buzzer=0;	
		}
		else{
			Buzzer=1;
		}
		if(second==0)//当second为0时，停止倒计时
		{
			flag=0;	 
			TR0=0;			
		}	
 	}	 
}

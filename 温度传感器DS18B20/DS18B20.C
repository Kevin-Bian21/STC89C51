#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit W_LE=P2^7;
sbit D_LE=P2^6;
sbit DS=P2^2;
uchar code Wei[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code Duan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X40};
void delay(uint z)
{
	uint i,y;
	for(i=z;i>0;i--)
		for(y=114;y>0;y--);
}
void delay_us(uchar us)
{	
	while(us--);
}
void display(uint k)   //动态显示自己设定想要显示的数字
{
	uint bai,shi,ge;
	bai=k/100;
	shi=k%100/10;
	ge =k%10;
	
	P0=0xff;
	W_LE=1;
	P0=Wei[5];
	W_LE=0;
	D_LE=1;
	P0=Duan[bai];
	D_LE=0;
	delay(5);
	
	P0=0xff;
	W_LE=1;
	P0=Wei[6];
	W_LE=0;
	D_LE=1;
	P0=Duan[shi]|0x80;
	D_LE=0;
	delay(5);

	P0=0xff;	
	W_LE=1;
	P0=Wei[7];
	W_LE=0;
	D_LE=1;
	P0=Duan[ge];
	D_LE=0;
	delay(5);		
}
bit DS_Init()
{
	bit i;
	DS=1;
	DS=0;
	delay_us(75);
	DS=1;
	delay_us(4);
	i=DS;
	delay_us(20);
	DS=1;
	_nop_();
	return(i);
}
	 
void write(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{  
		DS=0;
		_nop_();
		if(dat&0x01)
			DS=1;
		else
			DS=0;
		delay_us(10);
		DS=1;  //释放总线，以便下一次写入
		_nop_();
		dat>>=1;
	}
}
uchar read()
{
	uchar i;
	uchar dat;
	for(i=0;i<8;i++)
	{
		DS=0;
		_nop_();
		DS=1;
		_nop_();
		dat>>=1;
		if(DS)
			dat|=0x80;
		delay_us(10);
		DS=1;
		_nop_();
	}
	return(dat);
}
void main()
{
	uint i;
	uchar L,H;
	DS_Init();//初始化DS18B20
	write(0xcc);//发送跳跃ROM指令
	write(0x4e);//写暂存器指令
	write(0x7f);
	write(0xf7);
	write(0x1f);//配置工作在9位模式下
	DS_Init();//初始化DS18B20
	write(0xcc);//发送跳跃ROM指令 
	write(0x48);

	while(1)
	{
		DS_Init();
		write(0xcc);  //发送跳跃ROM指令
		write(0x44);  //发送温度转换指令
		DS_Init();
		write(0xcc);
		write(0xbe);  //读取DS18B20暂存器值
		L=read();
		H=read();
		i=H;
		i<<=8;
		i|=L;
		i=i*0.0625*10+0.5;
		display(i);
	}
}



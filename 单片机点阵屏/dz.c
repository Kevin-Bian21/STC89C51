#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit I_CP = P3^5;	//��������ʱ���ź�
sbit O_CP = P3^6;	//�������ʱ���ź�
sbit DS = P3^4;   //�������������
uint i,j,m,n,row;
uchar code a[2][8]=
{
0xEF,0xC7,0x83,0x01,0x01,0x93,0xFF,0xFF,
 0xFF,0xEF,0xC7,0x83,0x01,0x01,0x93,0xFF
}; 
/* 0xFF,0xEF,0xC7,0x83,0x01,0x01,0x93,0xFF				
void main()
{
	uchar i,j;
	i=0xfe;	//�У��͵�λ��Ч
	for(j=0;j<8;j++)
	{	
		I_CP=0;
		O_CP=0;
		if(i & 0x01)
			DS=1;
		else
			DS=0;
		I_CP=1;
		i>>=1;
	}
	i=0x40;	 //�У��ߵ�λ��Ч
	for(j=0;j<8;j++)
	{
		I_CP=0;
		O_CP=0;			

		if(i & 0x01)
			DS=1;
		else
			DS=0;
		I_CP=1;
		i>>=1;
	}
	O_CP=1;
	while(1);	
}
*/

void display(uchar z)
{	
	for(j=0;j<8;j++)
	{
		I_CP=0;
		if(z & 0x01)
			DS=1;
		else
			DS=0;
		I_CP=1;
		z>>=1;
	}
}
void main()
{  	
	P1=0x00;
	while(1){ 
		for(m=0;m<2;m++){ 
			for(n=0;n<500;n++){	  //ѭ����ʾ500��
				row=0x80;	
				for(i=0;i<8;i++){
					O_CP=0;
					display(a[m][i]);
					display(row);
					row=_cror_(row,1);
					O_CP=1;
				}
			}  
		}
	}
}
	









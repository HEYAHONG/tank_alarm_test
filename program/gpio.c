#include "stc15.h"
//-----------------------------------------------

#define FOSC 35000000L

#define T1MS (65536-FOSC/1000)      //1T模式
//#define T1MS (65536-FOSC/12/1000) //12T模式


sbit out=P3^4;
bit beep=0;
sbit out_r=P3^5;
sbit in=P3^3;
sbit f_out=P3^2;
#define count_total 64

void delay(unsigned long n)
{
while(n--);
}
//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
    if(beep) out_r = ! out_r;                    //将测试口取反
	else out_r=0;
}

//-----------------------------------------------



unsigned char count_0=0,count_1=0,temp=0;
void main()
{
	P3M1=0x00;
	P3M0=0x00;
	in=1;
   
    AUXR |= 0x80;                   //定时器0为1T模式
//  AUXR &= 0x7f;                   //定时器0为12T模式

    TMOD = 0x00;                    //设置定时器为模式0(16位自动重装载)
    TL0 = T1MS;                     //初始化计时值
    TH0 = T1MS >> 8;
    TR0 = 1;                        //定时器0开始计时
    ET0 = 1;                        //使能定时器0中断
    EA = 1;


while(1)
{
	f_out=1;
	in=1;
	delay(1000);
	f_out=0;
	delay(100);
	if(in) count_1++;
	else count_0++;

	if(temp==count_total)
	{
	if(count_0>=count_1) out=0,beep=0;
	else out=1,beep=1;
	count_0=count_1=temp=0;
	}
	else temp++;
}
}
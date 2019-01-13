#define uchar unsigned char
#define uint  unsigned int

sbit    int0_int=   P3^2;
sbit    l_button =  P2^0;
sbit    c_button=P2^1;
sbit    f_button=P2^6;
sbit    min_elect_c_button=P2^7;
sbit    max_elect_c_button=P3^6;
sbit    fangdian_button=P2^5;

void   judge_xianshi(void);
void   lx_display();
void   cx_display();
void   fx_display();
void   init_t0();

void    strive_f1();
void    strive_f2();
void    strive_cx();
void    strive_fx();
void    strive_min_c();
void    strive_max_c();
uchar t0_crycle=0;
uchar f_crycle;
uchar  flag1;//
uchar display_flag;
uint  f1,temp,f2;
long ryz=0;
unsigned int d;

extern void i2s(long num);

void   judge_xianshi()
{
    lx_display();
    cx_display();
    fx_display();
}
void    strive_max_c()//大电解电容测试
{
   max_elect_c_button=1;//置为1，准备判断小电容测量开关的状态，上电时这个开关比较弹起
   if(max_elect_c_button==0)
   {  
        f_crycle=0;
        fangdian_button=0;
        delayms(250);
        fangdian_button=1;
        TMOD=0x10;//设定T0以工作方式1定时
        TH1=0;
        TL1=0;
        EA=1;
        ET1=1;//允许定时器0中断
        TR1=1;
        int0_int=1;
        while(int0_int==1);
        TR1=0;
        ryz=0;
        ryz= f_crycle*50000;
        ryz+=TH1*256+TL1;

    LCD_String("Cx=",2,0);
	i2s(ryz);
	LCD_Write('u');
	LCD_Write('F');
	LCD_ClrStr(7);
   }   
}
void    strive_min_c()
{
   min_elect_c_button=1;
   if(min_elect_c_button==0)
   {  
        f_crycle=0;
        fangdian_button=0;
        delayms(250);
        fangdian_button=1;
        TMOD=0x10;
        TH1=0;
        TL1=0;
        EA=1;
	    ET1=1;//允许定时器0中断 
        TR1=1;
        display_flag=4;//显示标志，为4为测试小电解电容
        int0_int=1;
        while(int0_int==1);
        TR1=0;
        ryz=0;
        ryz+=50000*f_crycle;
        ryz+=TH1*256+TL1;
        ryz/=20;

	LCD_String("Cx=",2,0);
	i2s(ryz);
	LCD_Write('u');
	LCD_Write('F');
	LCD_ClrStr(7);
        display_flag=4;
   }   
}
void timer1() interrupt 3
{
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
    f_crycle++;
}
//求取小电容（无极性电容）的大小
void    strive_fx()
{
   uchar i;
   f_button=1;
   if(f_button==0)
   {
      if(display_flag!=3)init_t0();
      display_flag=3;
      TR0=1;
      TR1=1;
      ryz=0;
      for(i=0;i<20;i++)
      {
        f_crycle=0;
        while(f_crycle<1);
        f_crycle=0;
        ryz+=temp;
      }
      TR0=0;
      TR1=0;
    }
}
void timer0() interrupt 1
{
	TH0=(65536-50000)/256;//定时50毫秒
	TL0=(65536-50000)%256;
      f_crycle++;
	  t0_crycle=0;
	  TR0=0;//关闭定时0
      temp=TH1*256+TL1;//
      TH1=0;
      TL1=0;
      TR0=1;
}
void fx_display()
{
 
   if(display_flag==3)
   {
     LCD_String("Fx=",2,0);
	 if (ryz > 1000){

	 	d = ryz%1000;
		ryz /= 1000;
		i2s(ryz);
		LCD_Write('.');
		i2s(d);
	 	LCD_Write('k');
	 } 
	 else {i2s(ryz);}

	 LCD_Write('H');
	 LCD_Write('z');
	 LCD_ClrStr(7);

   }
}
void cx_display()
{
   if(display_flag==2)
   {
	LCD_String("Cx=",2,0);
	 if (ryz > 1000){

	 	d = ryz%1000;
		ryz /= 1000;
		i2s(ryz);
		LCD_Write('.');
		i2s(d);
	 	LCD_Write('n');

	 } 
	else {
		i2s(ryz);
		LCD_Write('p');
	}

	LCD_Write('F');
	LCD_ClrStr(7);
	ryz = 0;
   }
}
void   lx_display()
{
     if(display_flag==1)
     {
		LCD_String("Lx=",2,0);
		i2s(ryz);
		LCD_Write('u');
		LCD_Write('H');
		LCD_ClrStr(10);
     }
}
void init_t0()
{
    TMOD=0x51;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	t0_crycle=0;
    TH1=0;
    TL1=0;
}
void    strive_cx()
{
     
   c_button=1;
   if(c_button==0)
   {
      if(display_flag!=2)init_t0();
      display_flag=2;
      TR0=1;
      TR1=1;
      f_crycle=0;
      while(f_crycle<=10);
      f_crycle=0;
      TR0=0;
      TR1=0;
      f2=temp;
      if(f2>f1)f2=f1;
      if(f2<65)f2=65;
      if(f2!=0)
      {
        ryz=((unsigned long) f1)*((unsigned long )f1);
        ryz/=f2;
        ryz*=2000;
        ryz/=f2;
        ryz-=2000;
      }   
   }
}
void   strive_f2()
{
   l_button=1;
   if(l_button==0)
   {
      if(display_flag!=1)init_t0();
      display_flag=1;
      TR0=1;
      TR1=1;
      f_crycle=0;
      while(f_crycle<=15);
      f_crycle=0;
      TR0=0;
      TR1=0;
      f2=temp;
      if(f2>f1)f2=f1;
      if(f2<65)f2=65;
      if(f2!=0)
      {
        ryz=((unsigned long) f1)*((unsigned long )f1);
        ryz/=f2;
        ryz*=1000;
        ryz/=f2;
        ryz-=1000;
      }   
   }
}
void   strive_f1()
{
   uchar i;

	LCD_String("Calibration...",1,0);
	LCD_SetCursor(2,0);
   for(i=0;i<5;i++)
   {
      l_button=1;
      while(l_button==0);
      TR0=1;
      TR1=1;
      while(f_crycle<=10);
      f_crycle=0;
      TR0=0;
      TR1=0;
      f1=temp;
	  LCD_Write('#');
	  delayms(50);
   }

	LCD_Clear();
	delayms(300);

}

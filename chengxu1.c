#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int


//==============LCD1602�ӿ����ӷ���=====================
/*-----------------------------------------------------
       |DB0-----P0.0 | DB4-----P0.4 | RW-------P2.3    |
       |DB1-----P0.1 | DB5-----P0.5 | RS-------P2.4    |
       |DB2-----P0.2 | DB6-----P0.6 | E--------P2.2    |
       |DB3-----P0.3 | DB7-----P0.7 | 
    ---------------------------------------------------*/
//================================================*/              
#define LCM_Data     P0    //LCD1602���ݽӿ�
#define Busy         0x80   //���ڼ��LCM״̬���е�Busy��ʶ
sbit    LCM_RW     = P2^3;  //��д��������ˣ�LCD1602�ĵ����
sbit    LCM_RS     = P2^4;  //�Ĵ���ѡ������ˣ�LCD1602�ĵ��Ľ�
sbit    LCM_E      = P2^2;  //ʹ���ź������,LCD1602�ĵ�6��

sbit    int0_int=   P3^2;
sbit    l_button =  P2^0;//��в������ƿ��أ�
sbit    c_button=P2^1;//���ݲ������ƿ��أ�
sbit    f_button=P2^6;//Ƶ�ʲ��Կ��ƿ��أ�
sbit    min_elect_c_button=P2^7;//С���ݲ��Կ��ƿ���
sbit    max_elect_c_button=P3^6;//С���ݲ��Կ��ƿ���
sbit    fangdian_button=P2^5;//�����ݲ���ʱ�ŵ���ƽ�

//**************��������***************************************
void    WriteDataLCM		(uchar WDLCM);//LCDģ��д����
void    WriteCommandLCM	(uchar WCLCM,BuysC); //LCDģ��дָ��
uchar   ReadStatusLCM(void);//��LCDģ���æ��
void    DisplayOneChar(uchar X,uchar Y,uchar ASCII);//�ڵ�X+1�еĵ�Y+1λ����ʾһ���ַ�
void    LCMInit(void);//LCD��ʼ
void    delayms(uint ms);//1MS��׼��ʱ����
void    DisplayListChar(uchar X,uchar Y,uchar delayms, uchar code *DData);
void   judge_xianshi(void);//��ʾ�������
void   lx_display();
void   cx_display();
void   fx_display();
void   init_t0();

void    strive_f1();//��ȡF1
void    strive_f2();//��ȡ��еĴ�С
void    strive_cx();//��ȡС���ݣ��޼��Ե��ݣ��Ĵ�С
void    strive_fx();//�������Ƶ��
void    strive_min_c();//С���ݲ���
void    strive_max_c();//����ݲ���
uchar t0_crycle=0;
uchar f_crycle;
uchar  flag1;//
uchar display_flag;
uint  f1,temp,f2;
long ryz;
//***********************������******************************
main()   
{   
   fangdian_button=1;
   LCMInit();
   init_t0();
   strive_f1();//��ȡF1
   DisplayListChar(0,0,0, "F/L/C Tester");
   while(1)
   {
     strive_f2();//��ȡF1
     strive_cx();
     strive_fx(); 
     strive_min_c();
     strive_max_c();
     judge_xianshi();
   }
}
void   judge_xianshi()
{
    lx_display();
    cx_display();
    fx_display();
}
void    strive_max_c()//������ݲ���
{
   max_elect_c_button=1;//��Ϊ1��׼���ж�С���ݲ������ص�״̬���ϵ�ʱ������رȽϵ���
   if(max_elect_c_button==0)
   {  
        f_crycle=0;
        fangdian_button=0;
        delayms(250);
        fangdian_button=1;
        TMOD=0x10;//�趨T0�Թ�����ʽ1��ʱ
        TH1=0;
        TL1=0;
        EA=1;
        ET1=1;//����ʱ��0�ж�
        TR1=1;
        int0_int=1;
        while(int0_int==1);
        TR1=0;
        ryz=0;
        ryz= f_crycle*50000;
        ryz+=TH1*256+TL1;
        DisplayListChar(1,0,0, "Cx=");
        DisplayOneChar(1,3,  ryz/1000000%10+0x30);
        DisplayOneChar(1,4, ryz/100000%10+0x30);
        DisplayOneChar(1,5, ryz/10000%10+0x30);
        DisplayOneChar(1,6, ryz/1000%10+0x30);
        DisplayOneChar(1,7, ryz/100%10+0x30);
        DisplayOneChar(1,8,'.');
        DisplayOneChar(1,9, ryz/10%10+0x30);
        DisplayOneChar(1,10,ryz%10+0x30);
        DisplayListChar(1,11,0, "UF  ");
   }   
}
void    strive_min_c()//С���ݲ���
{
   min_elect_c_button=1;//��Ϊ1��׼���ж�С���ݲ������ص�״̬���ϵ�ʱ������رȽϵ���
   if(min_elect_c_button==0)
   {  
        f_crycle=0;
        fangdian_button=0;
        delayms(250);
        fangdian_button=1;
        TMOD=0x10;//�趨T0�Թ�����ʽ1��ʱ
        TH1=0;
        TL1=0;
        EA=1;
	    ET1=1;//����ʱ��0�ж� 
        TR1=1;
        display_flag=4;//��ʾ��־��Ϊ4Ϊ����С������
        int0_int=1;
        while(int0_int==1);
        TR1=0;
        ryz=0;
        ryz+=50000*f_crycle;
        ryz+=TH1*256+TL1;
        ryz/=20;
        DisplayListChar(1,0,0, "Cx=");
        DisplayOneChar(1,3,  ryz/1000000%10+0x30);
        DisplayOneChar(1,4, ryz/100000%10+0x30);
        DisplayOneChar(1,5, ryz/10000%10+0x30);
        DisplayOneChar(1,6, ryz/1000%10+0x30);
        DisplayOneChar(1,7, ryz/100%10+0x30);
        DisplayOneChar(1,8,'.');
        DisplayOneChar(1,9, ryz/10%10+0x30);
        DisplayOneChar(1,10,ryz%10+0x30);
        DisplayListChar(1,11,0, "UF  ");
        display_flag=4;//��ʾ��־��Ϊ4Ϊ����С������
   }   
}
void timer1() interrupt 3
{
	TH1=(65536-50000)/256;//��ʱ50����
	TL1=(65536-50000)%256;
    f_crycle++;
}
//��ȡС���ݣ��޼��Ե��ݣ��Ĵ�С
void    strive_fx()
{
   uchar i;
   f_button=1;//��Ϊ1��׼���ж�С���ݲ������ص�״̬���ϵ�ʱ������رȽϵ���
   if(f_button==0)
   {
      if(display_flag!=3)init_t0();
      display_flag=3;//��ʾ��־��Ϊ3Ϊ����Ƶ��
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
	TH0=(65536-50000)/256;//��ʱ50����
	TL0=(65536-50000)%256;
      f_crycle++;
	  t0_crycle=0;
	  TR0=0;//�رն�ʱ0
      temp=TH1*256+TL1;//
      TH1=0;
      TL1=0;
      TR0=1;
}
void fx_display()
{
   if(display_flag==3)
   {
     DisplayListChar(1,0,0, "Fx=");
     DisplayOneChar(1,3,  ryz/1000000%10+0x30);
     DisplayOneChar(1,4, ryz/100000%10+0x30);
     DisplayOneChar(1,5, ryz/10000%10+0x30);
     DisplayOneChar(1,6, ryz/1000%10+0x30);
     DisplayOneChar(1,7, ryz/100%10+0x30);
     DisplayOneChar(1,8, ryz/10%10+0x30);
     DisplayOneChar(1,9,ryz%10+0x30);
     DisplayListChar(1,10,0, "HZ    ");
   }
}
void cx_display()
{
   if(display_flag==2)
   {
     DisplayListChar(1,0,0, "Cx=");
     DisplayOneChar(1,3, ryz/1000000%10+0x30);
     DisplayOneChar(1,4, ryz/100000%10+0x30);
     DisplayOneChar(1,5, ryz/10000%10+0x30);
     DisplayOneChar(1,6, ryz/1000%10+0x30);
     DisplayOneChar(1,7, ryz/100%10+0x30);
     DisplayOneChar(1,8, ryz/10%10+0x30);
     DisplayOneChar(1,9, ryz%10+0x30);
     DisplayListChar(1,10,0, "pF    ");
   }
}
void   lx_display()
{
     if(display_flag==1)
     {
       DisplayListChar(1,0,0, "Lx=");
       DisplayOneChar(1,3,  ryz/10000000%10+0x30);
       DisplayOneChar(1,4,  ryz/1000000%10+0x30);
       DisplayOneChar(1,5, ryz/100000%10+0x30);
       DisplayOneChar(1,6, ryz/10000%10+0x30);
       DisplayOneChar(1,7, ryz/1000%10+0x30);
       DisplayOneChar(1,8, ryz/100%10+0x30);
       DisplayOneChar(1,9, ryz/10%10+0x30);
       DisplayOneChar(1,10,'.');
       DisplayOneChar(1,11,ryz%10+0x30);
       DisplayListChar(1,12,0, "uH  ");
     }
}
//********************************************************************************************
void init_t0()
{
    TMOD=0x51;//�趨T0�Թ�����ʽ1��ʱ50����,T1Ϊ��������������ʽ1
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;//�����ж�
	ET0=1;//����ʱ��0�ж�
	t0_crycle=0;//��ʱ���жϴ���������Ԫ
    TH1=0;
    TL1=0;
}
//��ȡС���ݣ��޼��Ե��ݣ��Ĵ�С
void    strive_cx()
{
     
   c_button=1;//��Ϊ1��׼���ж�С���ݲ������ص�״̬���ϵ�ʱ������رȽϵ���
   if(c_button==0)
   {
      if(display_flag!=2)init_t0();
      display_flag=2;//��ʾ��־��Ϊ1Ϊ����С����
      TR0=1;
      TR1=1;
      f_crycle=0;
      while(f_crycle<=10);
      f_crycle=0;
      TR0=0;
      TR1=0;
      f2=temp;//
      if(f2>f1)f2=f1;
      if(f2<65)f2=65;
      if(f2!=0)
      {
        ryz=((unsigned long) f1)*((unsigned long )f1);
        ryz/=f2;
        ryz*=2000;//����ΪPF
        ryz/=f2;
        ryz-=2000;
      }   
   }
}
//����
void   strive_f2()
{
   l_button=1;//��Ϊ1��׼���жϵ�в������ص�״̬���ϵ�ʱ������رȽϵ���
   if(l_button==0)
   {
      if(display_flag!=1)init_t0();
      display_flag=1;//��ʾ��־��Ϊ1Ϊ���Ե��
      TR0=1;
      TR1=1;
      f_crycle=0;
      while(f_crycle<=15);
      f_crycle=0;
      TR0=0;
      TR1=0;
      f2=temp;//
      if(f2>f1)f2=f1;
      if(f2<65)f2=65;
      if(f2!=0)
      {
        ryz=((unsigned long) f1)*((unsigned long )f1);
        ryz/=f2;
        ryz*=1000;//����ΪUH
        ryz/=f2;
        ryz-=1000;
      }   
   }
}
//******************************************************
//�ϵ��ʱ����ȡF1
void   strive_f1()
{
   uchar i;
   for(i=0;i<5;i++)
   {
      l_button=1;//��Ϊ1��׼���жϵ�в������ص�״̬���ϵ�ʱ������رȽϵ���
      while(l_button==0);
      TR0=1;
      TR1=1;
      while(f_crycle<=10);
      f_crycle=0;
      TR0=0;
      TR1=0;
      f1=temp;
   }
}
/*====================================================================  
  ��ָ��λ����ʾһ���ַ�:�� X ��,�� y��
  ע��:�ַ������ܳ���16���ַ�
======================================================================*/
void DisplayListChar(uchar X,uchar Y,uchar ms, uchar code *DData)
{
 unsigned char ListLength;

 ListLength = 0;

 X &= 0x1;
 Y &= 0xF; //����X���ܴ���15��Y���ܴ���1
 while (DData[ListLength]!='\0') //����ִ�β���˳�
  { 
     if (Y <= 0xF) //X����ӦС��0xF
     {
        DisplayOneChar(X, Y, DData[ListLength]); //��ʾ�����ַ�
        ListLength++;
        Y++;
	    delayms(ms);//��ʱ��ʾ�ַ���
     }
     else
	    break;//����ѭ���� 
  }
}
/*======================================================================
 LCM��ʼ��
======================================================================*/
void LCMInit(void) 
{
 LCM_Data = 0;
 WriteCommandLCM(0x38,0); //������ʾģʽ���ã������æ�ź�
 delayms(5);
 WriteCommandLCM(0x38,0);
 delayms(5);
 WriteCommandLCM(0x38,0);
 delayms(5);
 WriteCommandLCM(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 WriteCommandLCM(0x08,1); //�ر���ʾ
 WriteCommandLCM(0x01,1); //��ʾ����
 WriteCommandLCM(0x06,1); // ��ʾ����ƶ�����
 WriteCommandLCM(0x0C,1); // ��ʾ�����������
 delayms(100);
}
//==============================LCD1602��ʾ�ӳ���================================================
// д���ݺ���: E =������ RS=1 RW=0
//======================================================================*/
void WriteDataLCM(uchar WDLCM)
{
 ReadStatusLCM(); //���æ
 LCM_Data = WDLCM;
 LCM_RS = 1;
 LCM_RW = 0;
 LCM_E = 0; //�����ٶ�̫�߿���������С����ʱ
 LCM_E = 0; //��ʱ
 LCM_E = 1;
}
/*====================================================================
  дָ���: E=������ RS=0 RW=0
======================================================================*/
void WriteCommandLCM(uchar WCLCM,BuysC) //BuysCΪ0ʱ����æ���
{
 if (BuysC) ReadStatusLCM(); //������Ҫ���æ
 LCM_Data = WCLCM;
 LCM_RS = 0;
 LCM_RW = 0;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
}
/*====================================================================
  ������д����֮ǰ������LCD������״̬:E=1 RS=0 RW=1;
  DB7: 0 LCD���������У�1 LCD������æ��
  ��״̬
======================================================================*/
uchar ReadStatusLCM(void)
{
 LCM_Data = 0xFF;
 LCM_RS = 0;
 LCM_RW = 1;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
 while (LCM_Data & Busy); //���æ�ź�  
 return(LCM_Data);
}
/*======================================================================
�� ��:     ��1602 ָ��λ����ʾһ���ַ�:��һ��λ��0~15,�ڶ���16~31
˵ ��:     �� X ��,�� y ��  ע��:�ַ������ܳ���16���ַ�
======================================================================*/
void DisplayOneChar( uchar X, uchar Y, uchar ASCII)
{
  X &= 0x1;
  Y &= 0xF; //����Y���ܴ���15��X���ܴ���1
  if (X) Y |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
  Y |= 0x80; // ���ָ����
  WriteCommandLCM(Y, 0); //���ﲻ���æ�źţ����͵�ַ��
  WriteDataLCM(ASCII);
}
/*====================================================================
  �趨��ʱʱ��:x*1ms
====================================================================*/
void delayms(uint Ms)
{
  uint i,TempCyc;
  _nop_();
  for(i=0;i<Ms;i++)
  {
    TempCyc =70;
    while(TempCyc--);
  }
}

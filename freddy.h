#ifndef FREDDY_H
#define FREDDY_H

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
void   judge_xianshi(void);//��ʾ��������
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

#endif

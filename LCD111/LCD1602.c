#include <msp430.h>
#include <string.h>
#include <LCD1602.h>
#include <intrinsics.h>
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define DataDir     P3DIR
#define DataPort    P3OUT
#define Busy        0x80
#define CtrlDir     P1DIR
#define CLR_RS P1OUT&=~BIT3;    //RS = P1.3
#define SET_RS P1OUT|=BIT3;
#define CLR_RW P1OUT&=~BIT4;    //RW = P1.4
#define SET_RW P1OUT|=BIT4;
#define CLR_EN P1OUT&=~BIT5;    //EN = P1.5
#define SET_EN P1OUT|=BIT5;
void Disp1Char(unsigned char data)
{
    LCD1602_WriteDATA(data);
}
void DispStr(unsigned char *ptr)
{
    unsigned char i,n;
    n=strlen(ptr);
    for (i=0;i<n;i++)
    {
        Disp1Char(ptr[i]);
    }
}
void LCD1602_WriteCMD(unsigned char cmd)
{
        CLR_EN;
        delay_ms(10);
        CLR_RS;     //ָ��
        delay_ms(10);
        CLR_RW;     //д
        DataPort=cmd;   //ָ�����ݴ���P0�ڴ����͸�LCD1602
        delay_ms(10);
        SET_EN;     //LCD1602ʹ�ܿ�
        delay_ms(10);
        CLR_EN;     //�����½���
        delay_ms(10);
}
void LCD1602_WriteDATA(unsigned char Data)
    {
        SET_RS;
        delay_ms(10);
        CLR_EN;
        delay_ms(10);
        CLR_RW;
        DataPort=Data;
        SET_EN;
        delay_ms(10);
        CLR_EN;
        delay_ms(10);
    }
void LCD1602_Initi()
    {
        CtrlDir |= 0x38;        //�����߶˿���Ϊ���״̬
        DataDir  = 0xff;
        LCD1602_WriteCMD(0x38); //  �ù��ܣ�8λ���ߣ�˫����ʾ����ʾ5X7�ĵ����ַ�
        LCD1602_WriteCMD(0x0c); //  ��ʾ���ؿ���,����ʾ, �й��,��˸
        LCD1602_WriteCMD(0x06); //  ������ʾλ������,  ����ƶ���������,��Ļ���������ֲ���
        LCD1602_WriteCMD(0x01); //  ����ʾ��ָ����01H����긴λ����ַ00Hλ��
        delay_ms(10);
    }

#ifndef LCD12864_H_
#define LCD12864_H_
#include <msp430f5529.h>

#define CS_H P3OUT|=BIT3        //Ƭѡ�źţ�����Ч
#define CS_L P3OUT&=~BIT3

#define SID_H P3OUT|=BIT2       //����������
#define SID_L P3OUT&=~BIT2

#define SCLK_H P3OUT|=BIT1      //����ʱ������
#define SCLK_L P3OUT&=~BIT1

#define LCD_Pin_OUT P3DIR|=(BIT1+BIT2+BIT3);

#define uchar unsigned char
#define uint unsigned int

extern void write_cmd_data(uchar cmd_data,uchar cmd_or_data);	    //cmd_or_data,0��д���1��д����
extern void set_DDR_addr(uchar x,uchar y);                          //�趨DDRAM��ַ��x:������y������
extern void DispStr(uchar x,uchar y,char*ptr);                      //��ʾ�ַ�����x��yΪ0ʱ���趨�µ�ַ
extern void DispInt(uchar x,uchar y,uint number);                   //����ʾС��65536����Ȼ��������ֵΪ��Ȼ��λ����x��yΪ0ʱ���趨�µ�ַ
extern void LCD_Delay_us(uint time);                                //��ʱ����,��16MHz����
extern void LCD_Init(void);                                         //Һ����ʼ��
extern void LCD_Clear(void);                                        //Һ������
extern void disp_int_right(uchar x, uchar y, uint number);          //x,y Ϊ��ʼ��ַ����numberС��65536															//�Ҷ���
extern void disp_int_left(uchar x, uchar y, uint number);           //x,y Ϊ��ʼ��ַ����numberС��65536
extern void lcd_printf(char *s,int temp_data);
extern void lcd_printf1(char *s,int temp_data);
extern void LCD_Delay_us(uint time);
#endif /* LCD12864_H_ */

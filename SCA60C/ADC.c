/*
 * ADC.c
 *
 *  Created on: 2019��8��4��
 *      Author: ZZX
 */
#include "ADC.h"

void IO_init()
{
    P5SEL |= BIT0;
    P6SEL |= BIT0;
    P6DIR |= BIT5;//ģ���ź�����                                                                                   //����GPIO����
    P5DIR &= ~BIT0;//�ο��ߵ�ѹ����
}
void AD_Init()
{
    ADC12CTL0 |= ADC12MSC;                                                                              //�Զ�ѭ������ת��
    ADC12CTL0 |= ADC12ON;                                                                                   //����ADC12ģ��
    ADC12CTL1 |= ADC12CONSEQ1 ;                                                                     //ѡ��ͨ��ѭ������ת��
    ADC12CTL1 |= ADC12SHP;                                                                              //��������ģʽ
    ADC12MCTL0 |= ADC12INCH_0;                                                                      //ѡ��ͨ��5�����Ӳ����λ��
   // ADC12MCTL0 |= ADC12SREF_1;   //�ο���ѹ���1��VREF+(P5.0)  ~   AVSS(GND)
    ADC12CTL0 |= ADC12ENC;
}




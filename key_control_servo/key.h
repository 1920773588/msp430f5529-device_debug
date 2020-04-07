/*
 * key.h
 *
 *  Created on: 2019��8��7��
 *      Author: ZZX
 */
#include <msp430f5529.h>

#ifndef KEY_H_
#define KEY_H_

#define CPU_CLOCK 1000000               //Ĭ����Ƶ1Mhz
#define delay_ms(x) __delay_cycles((x)*CPU_CLOCK/1000);
#define delay_us(x) __delay_cycles((x)*CPU_CLOCK/1000000);
#define uchar unsigned char
#define KEYPORT P3OUT   //�����������
#define KEYSEL  P3SEL   //���ƿڹ���ѡ��io�ڹ���
#define KEYDIR  P3DIR  // ���ƿڷ���Ĵ���
#define KEYIN   P3IN   //��ȡio��ƽ
#define KEYREN  P3REN  //ʹ������������

void Port_Init(void);
uchar Key_Scan(uchar xx);

#endif /* KEY_H_ */

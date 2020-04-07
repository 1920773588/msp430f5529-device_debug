#include <msp430.h>


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR =BIT0|BIT6;
	P1OUT =BIT0;
	TA0CCR0 = 16384;                        // �����жϼ�������1s,ʱ��Ƶ��Ϊ32.768kHZ,32768 / 32768 = 1s//����9999��ִ��һ���ж�
	TA0CCTL0 = CCIE;                        // TA0CCR0����/�Ƚ��жϼĴ����ж�ʹ��
	TA0CTL = TASSEL_1 + MC_1 + TACLR ;//+ ID_3;       // TASSEL_1,ACLKʱ��Դ  MC_1,������ģʽ,��ѡʱ��Դ8

	_BIS_SR(LPM3_bits + GIE);
	return 0;
}
#pragma vector = TIMER0_A0_VECTOR;
__interrupt void Timer_A(void)              // ��ʱ���жϴ���,P3��������,��ƽ��ת
{
    P1OUT ^= BIT0;
    P1OUT ^= BIT6;
}

#include <msp430.h>
//��IO�жϿ���ʱ���ж�
int flag=0;
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT0+BIT6;
    P1DIR &=~BIT3;

    P1OUT |= BIT0;
    P1OUT &= ~BIT6;

    P1REN |= BIT3;
    P1IES &=~BIT3;
    P1IFG &=0x00;
    P1IE  |=BIT3; //������˸�ֵΪ1,��ʾ����IO�жϣ������¿���ʱ����ȥִ���жϺ��������û��IO�жϺ���������Ῠס

    TA0CTL |=MC_1+TASSEL_1+TACLR;
    TA0CCTL0 =CCIE;
    TA0CCR0 =9999;

//msp430��ָ���У�DINT��EINT�ֱ�ָ�غͿ������жϣ�Ҳ���ǰ���P1IE��P2IE��WDTIE��TAIE��TBIE��ADC12IE�������ж�����������ж�����λΪ��0����Ϊ��1��
    _EINT();//ȫ���жϻὫ�����ж�ʹ�ܿ���������IO�жϣ��������жϣ�������������ĳ���ж�ʹ��û�����ã����жϲ���������
    while(1){
        if(flag == 1){
            TA0CCTL0 &=~CCIE;//���������жϹر�
        }
        else{
            TA0CCTL0 =CCIE;//�������жϿ���
        }
    }

    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void){
    unsigned int i;
    for(i=0;i<12000;i++);//������ʮ����Ҫ
    flag=(flag+1)%2;
    P1IFG &=~BIT3;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    P1OUT ^=BIT0+BIT6;
}

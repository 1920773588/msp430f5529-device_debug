#include <msp430.h>
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

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    //P1REN |= BIT3;
    //P1IES &=~BIT3;
   // P1IFG &=0x00;
   // P1IE  |=BIT3; //������˸�ֵΪ1,��ʾ����IO�жϣ������¿���ʱ����ȥִ���жϺ��������û��IO�жϺ���������Ῠס


    TA0CTL |=MC_1+TASSEL_2+TACLR+ID_3;
    TA0CCTL0 =CCIE;
    TA0CCR0 =124999;

    _EINT();

    while(1){
        if(P1IN & BIT3 != BIT3){
            TA0CCTL0 &=~CCIE;
        }
        else{
            TA0CCTL0 =CCIE;
        }

    }

    return 0;
}
/*
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void){
    unsigned int i;
    for(i=0;i<12000;i++);//������ʮ����Ҫ
    flag=(flag+1)%2;
    P1IFG &=~BIT3;
}*/

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    P1OUT ^=BIT0+BIT6;
}
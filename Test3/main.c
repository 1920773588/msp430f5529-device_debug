#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |=BIT0+BIT6;
    P1DIR &=~BIT3;//����Ϊ����
    P1OUT |=BIT0+BIT6;

    P1REN |=BIT3; //����1.3����
    P1OUT |=BIT3;//������������
    P1IES &=~BIT3;//�жϱ�Եѡ��0Ϊ�½��أ�1Ϊ������
    P1IFG &=~BIT3;//���1.3�жϱ�־
    P1IN  |=BIT3;//�ж�ʹ��
    P1IE  |=BIT3;

    _EINT();//����ȫ���ж�

    //__bis_SR_register(LPM4_bits+GIE);//�����͹���ģʽ��ȫ���ж�
    //__no_operation();
    /*
    while(1){
        if(P1IN & BIT3){
            P1OUT &=~(BIT0+BIT6);
        }
        else
            P1OUT |=BIT0+BIT6;
    }*/
    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void){
    P1OUT ^=BIT0+BIT6;
    P1IFG &=~BIT3;
}

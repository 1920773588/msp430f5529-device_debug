#include <msp430.h>
#include <msp430f5529.h>
unsigned int i = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    P8DIR |= BIT1+BIT2;
    P8OUT |= BIT1;
    P8OUT &= ~BIT2;
    TA0CTL |= MC_2 + TASSEL_1 + TACLR;      //������ģʽ��ʱ��ΪSMCLK,�Ƚ�ģʽ����ʼʱ���������
    TA0CCR0 = 0xffff;
    TA0CCTL1 = CCIE;                        //�Ƚ����ж�ʹ��
    TA0CCR1  = 32768;                         //�Ƚ�ֵ��Ϊ50000���൱��50ms��ʱ����
    __enable_interrupt();
    /*
	  P6DIR |= BIT0+BIT1;
	  P6OUT |= BIT1;
	  P6OUT &=~BIT0;
	  P2DIR |= BIT0+BIT5;                       // P2.0 and P2.1 output
	  P2SEL |= BIT0+BIT5;                       // P2.0 and P2.1 options select
	  TA1CCR0 = 1000;                          // PWM Period
	  TA2CCR0 = 1000;
	  TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
	  TA1CCR1 = 1000;                            // CCR1 PWM duty cycle
	  TA2CCTL2 = OUTMOD_7;                      // CCR2 reset/set
	  TA2CCR2 = 700;                            // CCR2 PWM duty cycle
	  TA1CTL = TASSEL_1 + MC_1 + TACLR; // ACLK, up mode, clear TAR
	  TA2CTL = TASSEL_1 + MC_1 + TACLR;

	  __bis_SR_register(LPM3_bits);             // Enter LPM3
	  __no_operation();*/

	while(1);
	return 0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A (void)
{
    switch(__even_in_range(TA0IV,14))

       {

          case 2:
              if(i == 5)
              {
                  i=0;
                  P8OUT ^= BIT1+BIT2;
              }
              i++;
              break; //TACCR1 CCIFG��λ����������ֵ���趨��13107����ˣ�Ҳ����˵����0.4S��

          case 4:P1OUT=BIT2;break; //TACCR2 CCIFG��λ����������0.8S��

          case 6:P1OUT=BIT3;break; //TACCR3 CCIFG��λ����������1.2S��

          case 8:P1OUT=BIT4;break; //TACCR4 CCIFG��λ����������1.6S��

          case 14:P1OUT=BIT5;break;   //TAIFG��λ����������2S��

          default:break;

       }
    //i++;
  //  if(i == 20){
      //  P8OUT ^= BIT1;
      //  i=0;
   // }

}


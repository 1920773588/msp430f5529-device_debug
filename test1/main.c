#include <MSP430g2553.h>

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |= BIT6+BIT0;         //P1��6,P1.0Ϊ���
  P1OUT &=~ BIT3;
  P1OUT |=BIT6+BIT0;          //P1.6,P1.0Ϊ�ߵ�λÿ��ѭ����������ø�

unsigned char Key=P1IN&BIT3;//������һ�εļ�ֵ
  while(1)
  {


    if(P1IN & BIT3)//�ϴ��뱾�μ�ֵ��ͬ���ұ��μ�ֵΪ���µ�ʱ�򣨼��������µ�ʱ��

    {
      P1OUT &=~( BIT6+BIT0);//�ı�����/�����ƽ
    }

     Key=P1IN&BIT3;//�����ΰ�����ֵ������
  }
}

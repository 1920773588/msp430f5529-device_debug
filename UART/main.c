/*****�ô������� ��ʲô  �ظ�ʲô****/
#include "msp430f5529.h"
// ACLK = REFO = 32768Hz, MCLK = SMCLK = default DCO/2 = 1048576Hz
// P3.4��5����USCI_A0 TXD/RXD��P9.4,5����USCI_A2 TXD/RXD��P10.4,5����USCI_A3 TXD/RXD��
unsigned int table[12]={1,2,3,4,5,6,7,8,9,10,11,12};
unsigned int j;
void Delay(unsigned int time)
{
  unsigned int i,k;
  for(i=0;i<255;i++)
    for(k=0;k<time;k++)
      _NOP();
}

void send_buf(unsigned char *ptr)
{
    while(*ptr !='\0') //C�������ַ���ĩβ�Զ��ӡ�\0��(ASCII��ֵΪ0)
    {
          while (!(UCA1IFG&UCTXIFG));
          UCA1TXBUF=*ptr; //�����ַ���Ӧ��ASCII�룬12864ָ��������Զ�ָ����һ����ʾ��ַ
          *ptr++;
          Delay(50);
    }
}

void initClock()
{
     UCSCTL6 &= ~XT1OFF; //����XT1
     P5SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
     UCSCTL6 &= ~XT2OFF;          //��XT2 = 4MHz
     __bis_SR_register(SCG0);
     UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
     UCSCTL1 = DCORSEL_4;       //DCOƵ�ʷ�Χ��28.2MHZ����
     UCSCTL2 = FLLD_4 + 1;       //D=16��N=1
     UCSCTL3 = SELREF_5 + FLLREFDIV_3;    //n=8,FLLREFCLKʱ��ԴΪXT2CLK��DCOCLK=D*(N+1)*(FLLREFCLK/n) = 16MHz;DCOCLKDIV=(N+1)*(FLLREFCLK/n) = 1MHz;
     UCSCTL4 = SELA_4 + SELS_3 +SELM_3;    //ACLK��ʱ��ԴΪDCOCLKDIV,MCLK\SMCLK��ʱ��ԴΪDCOCLK
     UCSCTL5 = DIVA_5 +DIVS_1;      //ACLK��DCOCLKDIV��32��Ƶ�õ���SMCLK��DCOCLK��2��Ƶ�õ�
                 //����MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ

   __bic_SR_register(SCG0);                   //Enable the FLL control loop

}

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR|=BIT0;
  P1OUT|=BIT0;
  P2IE  |= BIT1; //����p2.1�����ж�
  P2IES |= BIT1; //����p2.1Ϊ�½����ж�
  P2IFG &= ~BIT1;//����p2.1Ϊ0 ���ж�����
  P2REN |= BIT1; //����p2.1Ϊ����������ʹ��
  P2OUT |= BIT1;
  initClock();
  P4SEL |=BIT4+BIT5 ;                             // P5.6,7 = USCI_A1 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL__SMCLK;                     // SMCLK
  UCA1BR0 = 4;//175;//69;                              // 1MHz 115200 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 115200
  UCA1MCTL |= 0x51;//UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}
// Echo back RXed character, confirm TX buffer is ready first����������֮ǰȷ�����ͻ���׼����
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch(__even_in_range(UCA1IV,4))
      {
      case 0:break;                             // Vector 0 - no interrupt
      case 2://���ݽ����ж�
      while(!(UCA1IFG&UCTXIFG));
      UCA1TXBUF = UCA1RXBUF;//�����ջ��������ַ����͸����ͻ�����

      char temp = UCA1RXBUF;

      P8DIR |= BIT1;
      P3DIR |= BIT7;
      P7DIR |= BIT4;

      if( temp == 'a')
      {
          P8OUT ^= BIT1;
      }
      else if(temp == 'b')
      {
          P3OUT ^= BIT7;
      }
      else if(temp == 'c')
      {
          P7OUT ^= BIT4;
      }
      break;
      case 4:break;                             // Vector 4 - TXIFG
      default: break;
      }

}
// UCTXIFG=0x02��UCA1IFG&UCTXIFG����UCA1IFG��UCTXIFGλΪ1ʱ��˵��UCA1TXBUFΪ�գ�
//����whileѭ��ѭ������UCTXIFGλΪ0ʱUCA1TXBUF��Ϊ�գ�ͣ��ѭ����

#pragma vector=PORT2_VECTOR
__interrupt void Key_interrput()
{
    if(P2IFG&BIT1)
    {
      Delay(50);
      if(P2IFG&BIT1)
      {
          while((P2IN&BIT1)==0);//���ּ��
          P2IFG &= ~BIT1;  //����жϱ�־λ
          send_buf("zhu zi xuan\r\n");
      }
    }

}

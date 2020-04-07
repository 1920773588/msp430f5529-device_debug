/*
 * Config.c
 *
 *  Created on: 2020��1��13��
 *      Author: ZZX
 */
#include "Config.h"
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
void IO_init(void)
{
    P1DIR |= BIT2+BIT3+BIT4+BIT5+BIT6;
    P2DIR &= ~BIT0;
}
void ini_UART(){
          P4SEL |= BIT5+BIT4;                       // P4.4,5ʹ�����蹦�� = UCA1 TXD/RXD
          UCA1CTL1 |= UCSWRST;                      // ��λUSCI
          UCA1CTL1 |= UCSSEL__SMCLK;                     // ����SMCLKʱ�ӣ����ڷ����ض�������
          UCA1BR0 = 4;                            // ���ò����ʣ� 8MHz ������= 115200
          UCA1BR1 = 0;
          UCA1MCTL |= 0x51;//UCBRS_1 + UCBRF_0;
          UCA1CTL1 &= ~UCSWRST;                     // ������λ
          UCA1IE |= UCRXIE;
}
void driver_delay_us(unsigned int xus)  //1us
{
    for(;xus>1;xus--);
}

void driver_delay_xms(unsigned long xms) //1ms
{
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
    {
        for(i=0; i<256; i++);
    }
}
void DELAY_S(unsigned int delaytime)     //  1s
{
    int i,j,k;
    for(i=0;i<delaytime;i++)
  {
        for(j=0;j<4000;j++)
        {
            for(k=0;k<222;k++);

        }
    }
}
void DELAY_M(unsigned int delaytime)     //  1M
{
    int i;
    for(i=0;i<delaytime;i++)
        DELAY_S(60);
}



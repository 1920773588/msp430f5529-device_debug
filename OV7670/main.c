#include <msp430f5529.h>
#include "ov7670.h"
#include "SCCB.h"

unsigned int ov_sta = 0;
/**
 * main.c
 */

void initClock()
{
     UCSCTL6 &= ~XT1OFF; //����XT1
     P5SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
     UCSCTL6 &= ~XT2OFF;          //��XT2
     __bis_SR_register(SCG0);
     UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
     UCSCTL1 = DCORSEL_4;       //DCOƵ�ʷ�Χ��28.2MHZ����
     UCSCTL2 = FLLD_5 + 1;       //D=16��N=1
     UCSCTL3 = SELREF_5 + FLLREFDIV_3;    //n=8,FLLREFCLKʱ��ԴΪXT2CLK��DCOCLK=D*(N+1)*(FLLREFCLK/n);DCOCLKDIV=(N+1)*(FLLREFCLK/n);
     UCSCTL4 = SELA_4 + SELS_3 +SELM_3;    //ACLK��ʱ��ԴΪDCOCLKDIV,MCLK\SMCLK��ʱ��ԴΪDCOCLK
     UCSCTL5 = DIVA_5 +DIVS_1;      //ACLK��DCOCLKDIV��32��Ƶ�õ���SMCLK��DCOCLK��2��Ƶ�õ�
                 //����MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initClock();
	while(ov7670_init());
	//unsigned char  data_fifo[2];
	unsigned char FIFO_1,FIFO_2,FIFO_data;
	unsigned char img[piexl_h*piexl_w][2];
	P2IE |= BIT0;
	P2IFG &=~BIT0;//����жϱ�־
	P2IES &=~BIT0;//�������ж�
	/* OE  AL422 FIFO�����ʹ������ ��OEΪ�͵�ƽʱ������������� ��
	  �ߵ�ƽʱ�������������̬*/
	OE_L;
	if(ov_sta == 2)//������
	      {
	        P2IE &= ~BIT0;//���ⲿ�ж�
	        //����ͼ��ֱ���
	        OV7670_Window_Set(180,10,piexl_w,piexl_h);

	        RRST_L;//��ʼ��λ��ָ��
	        RCLK_L;
	        RCLK_H;
	        RCLK_L;
	        RRST_H;//��ָ�븴λ����
	        RCLK_H;

	        int j=0;
	        while(j<piexl_h*piexl_w){
	            RCLK_L;
	            FIFO_1 = P4IN&0x0f;//��ȡ���ֽ�
	            FIFO_2 = P6IN&0x0f;
	            FIFO_data = (FIFO_2<<4)|FIFO_1;
	            img[j][0] =  FIFO_data;
	           // data_fifo[0] = FIFO_data;
	            RCLK_H;
	            RCLK_L;
	            FIFO_1 = P4IN&0xf0;//��ȡ���ֽ�
	            FIFO_2 = P6IN&0xf0;
	            FIFO_data = (FIFO_2<<4)|FIFO_1;
	            img[j][1] =  FIFO_data;
	           // data_fifo[1] = FIFO_data;
	            RCLK_H;
	            j++;
	          }
	        j=0;
	        ov_sta = 0;
	        P2IE |= BIT0;
	     }
	return 0;
}

//FIFOģ��洢����ͷ����
#pragma vector = PORT2_VECTOR
__interrupt void PORT_2(void)
{
      WRST_L;//��ʼ��λдָ��
      WRST_H;//дָ�븴λ����
      if(ov_sta == 0)
      {
        WEN_H;
        ov_sta = 1;
      }
      else if(ov_sta == 1)
      {
        WEN_L;
        ov_sta = 2;
      }
      P2IFG &=~BIT0;     //�����־λ
}

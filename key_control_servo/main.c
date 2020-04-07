#include <msp430f5529.h>
#include "key.h"
#include "lcd_1602.h"
/**
 * main.c
 */
void initClock()
{
     UCSCTL6 &= ~XT1OFF; //����XT1
     P5SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
     UCSCTL6 &= ~XT2OFF;          //��XT2 = 4MHz
     UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
     UCSCTL1 = DCORSEL_4;       //DCOƵ�ʷ�Χ��28.2MHZ����
     UCSCTL2 = FLLD_4 + 1;       //D=16��N=1
     UCSCTL3 = SELREF_5 + FLLREFDIV_3;    //n=8,FLLREFCLKʱ��ԴΪXT2CLK��DCOCLK=D*(N+1)*(FLLREFCLK/n) = 16MHz;DCOCLKDIV=(N+1)*(FLLREFCLK/n) = 1MHz;
     UCSCTL4 = SELA_4 + SELS_3 +SELM_3;    //ACLK��ʱ��ԴΪDCOCLKDIV,MCLK\SMCLK��ʱ��ԴΪDCOCLK
     UCSCTL5 = DIVA_5 +DIVS_1;      //ACLK��DCOCLKDIV��32��Ƶ�õ���SMCLK��DCOCLK��2��Ƶ�õ�
                 //����MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ
     __bic_SR_register(SCG0);                   //Enable the FLL control loop

}
void servo_init(){
     P2DIR |= BIT0;                            //P2.0���PWM��
     TA0CCTL1 = OUTMOD_7;                         //�����ģʽѡ��Ϊģʽ7
     TA0CTL = TASSEL_1 + MC_1 + TACLR ;         //ѡ��ACLKʱ��Դ��timerA����ģʽѡ��upģʽ��������TA0CCR0������timerA������
     TA1CCTL1 = OUTMOD_7;
     TA1CTL = TASSEL_1 + MC_1 +TACLR ;
}
void set_PWM(unsigned int angle,char dir){
              //900΢�룬���õ�Ϊ1000΢�룬ʱ��Ƶ��Ϊ32768Hz
    TA0CCR0 = 20000;          //��֤����״̬1
    TA0CCR1 = 10000;
    TA1CCR0 = 100;              //���PWM������
    if(dir == 'A')
        TA1CCR1 = 45+angle*0.38;              //ռ�ձ�
    else if(dir == 'B')
        TA1CCR1 = 45-angle*0.38;
    P1DIR |=BIT2;
    P1SEL |=BIT2;
    P2DIR |=BIT0;
    P2SEL |=BIT0;
    //delay_ms(300);
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    uchar key = 0;
    Port_Init();//���̳�ʼ��
    init_lcd1602();//lcd1602��ʼ��
    servo_init();

    unsigned int angle1 = 0;
    char dir;

    while(1){
        write_cmd(0x01);
        write_cmd(0x80);
        angle1=0;
        while(1){
           key = Key_Scan(0);
           if(key == '#')
              break;
           else if(key>=0x30&&key<=0x39){
              angle1 = angle1*10+(key-0x30);
              write_data(key);
           }else if(key == 'A'||key == 'B'){
               dir = key;
               write_data(key);
           }
          delay_ms(1000);
       }
       set_PWM(angle1,dir);
    }

	return 0;
}

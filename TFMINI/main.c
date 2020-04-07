#include <msp430.h> 
#include "lcd_1602.h"
#include "key.h"
unsigned char data[7];
unsigned char recv_data;
unsigned int index = 0,i=0;
unsigned int start,flag=0;
unsigned int distance=0,strength=0;
unsigned int stop_flag = 0;
unsigned int time = 0;
void initClock()
{
     UCSCTL6 &= ~XT1OFF; //����XT1
     P5SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
     UCSCTL6 &= ~XT2OFF;          //��XT2 = 4MHz
     UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
     UCSCTL1 = DCORSEL_4;       //DCOƵ�ʷ�Χ��28.2MHZ����
     UCSCTL2 = FLLD_4 + 1;       //D=16��N=1
     UCSCTL3 = SELREF_5 + FLLREFDIV_3;    //n=8,FLLREFCLKʱ��ԴΪXT2CLK��DCOCLK=D*(N+1)*(FLLREFCLK/n) = 16MHz;DCOCLKDIV=(N+1)*(FLLREFCLK/n) = 1MHz;
     UCSCTL4 = SELA_4 + SELS_3 +SELM_4;    //ACLK��ʱ��ԴΪDCOCLKDIV,MCLK\SMCLK��ʱ��ԴΪDCOCLK
     UCSCTL5 = DIVA_5;//+DIVS_1;//+DIVM_2;      //ACLK��DCOCLKDIV��32��Ƶ�õ���SMCLK��DCOCLK��2��Ƶ�õ�
                 //����MCLK:16MHZ,SMCLK:1MHZ,ACLK:32KHZ
     __bic_SR_register(SCG0);                   //Enable the FLL control loop

}

void servo_init(){
    P2DIR |= BIT0;
    P1DIR |= BIT2;
    P2SEL |= BIT0;
    P1SEL |= BIT2;

    TA0CCTL1 = OUTMOD_7;                         //�����ģʽѡ��Ϊģʽ7
    TA0CTL = TASSEL_1 + MC_1 + TACLR ;         //ѡ��ACLKʱ��Դ��timerA����ģʽѡ��upģʽ��������TA0CCR0������timerA������
    TA0CCR0 = 100;
    TA0CCR1 = 70;

    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_1 + MC_1 +TACLR ;
    set_PWM(0,'A');
}

void ini_UART(){
          P4SEL |= BIT5+BIT4;                       // P4.4,5ʹ�����蹦�� = UCA1 TXD/RXD
          UCA1CTL1 |= UCSWRST;                      // ��λUSCI
          UCA1CTL1 |= UCSSEL__SMCLK;                     // ����SMCLKʱ�ӣ����ڷ����ض�������
          UCA1BR0 = 9;                            // ���ò����ʣ� 8MHz ������= 115200
          UCA1BR1 = 0;
          UCA1MCTL |= 0x02;//UCBRS_1 + UCBRF_0;
          UCA1CTL1 &= ~UCSWRST;                     // ������λ
          //UCA1IE |= UCRXIE;
}

void set_PWM(unsigned int angle,char dir){
              //900΢�룬���õ�Ϊ1000΢�룬ʱ��Ƶ��Ϊ32768Hz
    P2DIR |= BIT0;
    P1DIR |= BIT2;
    P2SEL |= BIT0;
    P1SEL |= BIT2;
    TA1CCR0 = 100;              //���PWM������
    TA1CCR1 = 46;
    if(dir == 'A')
        TA1CCR1 = 46+angle*0.36;              //ռ�ձ�
    else if(dir == 'B')
        TA1CCR1 = 46-angle*0.37;
    //P2DIR |=BIT0;
   // P2SEL |=BIT0;
   // delay_ms(100000);
}

void shot_dis(unsigned int dis){

    if(dis>=80 && dis<100){
        time = 1380;
    }
    else if(dis>=100 && dis<=120)
    {
        time = 1680;
    }else if(dis>120 && dis<=140){
        time = 1800;
    }else if(dis>140 && dis<=160){
        time = 2000;
    }else if(dis>160 && dis<=180){
        time = 2200;
    }else if(dis>180 && dis<=200){
        time = 2400;
    }else if(dis>200 && dis<=220){
        time = 2600;
    }else if(dis >220 && dis<=240){
        time = 2800;
    }else if(dis >240 && dis<=260){
        time = 3000;
    }else if(dis > 260 && dis <=280){
        time = 3200;
    }else if(dis > 280 && dis <=300){
        time = 3400;
    }else if(dis > 300 && dis <=320){
        time = 3600;
    }
    else if(dis > 320 && dis <=340){
            time = 3800;
        }
    else if(dis > 340 && dis <=360){
            time = 4000;
        }
    P8OUT |= BIT1;
    P8OUT &= ~BIT2;
    TA0CCTL2 = CCIE;
    TA0CCR2  = 99;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
   // initClock();
    ini_UART();
    Port_Init();//���̳�ʼ��
    init_lcd1602();//lcd1602��ʼ��
    servo_init();//�����ʼ��

   // TA0CTL |= MC_1 + TASSEL_1 + TACLR;      //������ģʽ��ʱ��ΪACLK,�Ƚ�ģʽ����ʼʱ���������
    //TA0CCTL0 = CCIE;                        //�Ƚ����ж�ʹ��
    //TA0CCR0  = 32767;                         //�Ƚ�ֵ��Ϊ50000���൱��50ms��ʱ����
    P8DIR |= BIT1+BIT2;
    P8OUT &= ~BIT1;
    P8OUT &= ~BIT2;
    unsigned int angle1 = 0,dis = 0;
    unsigned char key;
    char dir;
    while(1){
           write_cmd(0x01);
           P8OUT &= ~BIT2;
           display("INPUT QUESTION",0);
           display("NUMBER:",1);
           while(Key_Scan(0) == 0);
           key = Key_Scan(0);
           write_data(key);
           if(key == '1'){
               TA0CCTL2 = CCIE;                        //�Ƚ����ж�ʹ��
               TA0CCR2  = 99;
              // TA0CCTL0 = CCIE;
               time = 1638;
               P8OUT |= BIT1;
               P8OUT &= ~BIT2;
               __bis_SR_register(GIE);
           }
           else if(key == '2')////////////////����2
           {
               __bis_SR_register(GIE);
               dis = 0;
               write_cmd(0x01);
               display("DISTANCE(cm):",0);
               while(1){
                 key = Key_Scan(0);
                 if(key == '#')
                     break;
                 else if(key>=0x30&&key<=0x39){
                     dis = dis*10+(key-0x30);
                     write_data(key);
                 }
                 delay_ms(50);
               }
               shot_dis(dis);
           }
           else if(key == '3')
           {    ////////////////////����3
               __bis_SR_register(GIE);
               dis = 0;
               angle1 = 0;
               write_cmd(0x01);
               display("DISTANCE(cm):",0);
               while(1){
                 key = Key_Scan(0);
                 if(key == '#')
                     break;
                 else if(key>=0x30&&key<=0x39){
                     dis = dis*10+(key-0x30);
                     write_data(key);
                 }
                 delay_ms(50);
               }
           display("ANGLE:",1);
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
             delay_ms(50);
          }
          set_PWM(angle1,dir);
          shot_dis(dis);
         }
         else if(key == '4')
         {__bis_SR_register(GIE);
             UCA1IE |= UCRXIE;
             __bis_SR_register(GIE);
             stop_flag = 1;
             unsigned int i = 30;

             while(i>0 && stop_flag == 1){
                 set_PWM(i,'A');
                 delay_ms(500);
                 i--;
             }
             while(i<30 && stop_flag == 1){
                 set_PWM(i,'B');
                 delay_ms(500);
                 i++;
             }

         }
         else if(key == '5')
         {
             __bis_SR_register(GIE);
             unsigned int i = 30;

             while(i>0 ){
                 set_PWM(i,'A');
                 delay_ms(500);
                 i--;
             }
             while(i<30){
                 set_PWM(i,'B');
                 delay_ms(500);
                 i++;
             }
             set_PWM(20,'A');
             shot_dis(250);
         }
      }
}

#pragma vector=USCI_A1_VECTOR                                       //USCI�жϷ�����
__interrupt void USCI_A1_ISR(void)
{
switch(__even_in_range(UCA1IV,4))
{
    case 0: P1OUT &= ~BIT0;break;                                                   //���ж�
    case 2:                                                         //�����жϴ���
    {
        if(UCRXIFG){//�ȴ���ɽ���
            recv_data = UCA1RXBUF;
            start |= recv_data;
            if(start == 0x5959){
                flag = 1;
                break;
            }
            if(flag == 1){
                data[index] = recv_data;
                index++;
            }
            if(index == 8){
                flag =0;
                index = 0;
                start = 0;
                distance = data[1];
                distance = (distance<<8)|data[0];
                if(distance < 300)
                {
                    //TA0CTL |= MC_1 + TASSEL_1 + TACLR;      //������ģʽ��ʱ��ΪACLK,�Ƚ�ģʽ����ʼʱ���������
                 //   TA0CCR2  = 20000;                         //�Ƚ�ֵ��Ϊ50000���൱��50ms��ʱ����
                //    TA0CCTL2 |= CCIE;
                    P8OUT |= BIT1;
                   // P8OUT &= ~BIT2;
                    shot_dis(distance);
                   // UCA1IE &= ~UCRXIE;
                    stop_flag = 0;
                }
                strength = data[3];
                strength = (strength<<8)|data[2];
                write_cmd(0x01);
                unsigned char s[3];
                lcd_printf(s,distance);
                display(s,0);
                lcd_printf1(s,strength);
                display(s,1);
                }
                start = start<<8;
            }
        }
        break;
    case 4: P1OUT &= ~BIT0;break;                                                   //�����жϲ�����
    default:break;                                                  //��������޲���
}
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A (void)
{
    switch(__even_in_range(TA0IV,14))
       {
          case 2:break; //TACCR1 CCIFG��λ
          case 4:
              if(i==time)
              {
                  P8OUT &= ~BIT1;
                  P8OUT |= BIT2;
                 // UCA1IE |= UCRXIE;
                  TA0CCTL2 &= ~CCIE;
                  i=0;
              }else
                  i++;
              break; //TACCR2 CCIFG��λ
          case 6:break; //TACCR3 CCIFG��λ
          case 8:break; //TACCR4 CCIFG��λ
          case 14:break;   //TAIFG��λ
          default:break;
       }
}
/*
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)

{
                if(i==10)
                 {
                     P8OUT &= ~BIT1;
                     P8OUT |= BIT2;
                    // UCA1IE |= UCRXIE;
                     TA0CCTL2 &= ~CCIE;
                     i=0;
                 }
                else
                 i++;
}

*/

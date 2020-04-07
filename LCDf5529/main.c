#include <msp430.h> 
#include<string.h>

#define RS P3OUT //3.0
#define RW P3OUT //3.1
#define EN P3OUT //3.2
//#define data_port P6OUT P7.0
unsigned char buf[] = "WELCOME MSP430";
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
     //����MCLK:1MHZ,SMCLK:8MHZ,ACLK:32KHZ

     __bic_SR_register(SCG0);                   //Enable the FLL control loop

}
void initport(){
    P3SEL&=0Xf8;                  //����P3��ΪI/Oģʽ
    P6SEL&=0X00;                  //����P6��ΪI/Oģʽ
    P7SEL&=0xfe;
    P3DIR|=0X07;                  //����P3��Ϊ���ģʽ
    P6DIR|=0XFF;                  //����P6��Ϊ���ģʽ
    P7DIR|=0X01;
    P3OUT&=0X07;
    P6OUT&=0X00;
    P7OUT&=0x01;
}
void delay_1ms(int z)       //��ʱ1ms
{
    int i,j;
    for(i=0;i<z;i++)
        for(j=0;j<110;j++);
}
void write_com(unsigned char com){//д�����
    RS&=~BIT0;
    RW&=~BIT1;
    delay_1ms(5);
    EN&=~BIT2;
    P6OUT = com;
    P7OUT = com>>7;
    EN|=BIT2;
    delay_ms(5);
    EN&=~BIT2;
    RW |= BIT1;

}
void write_data(unsigned char data){//д���ݺ���
    RS|=BIT0;
    RW&=~BIT1;
    delay_1ms(5);
    EN&=~BIT2;
    P6OUT = data;
    P7OUT = data>>7;
    EN|=BIT2;
    delay_ms(5);
    EN&=~BIT2;
    RW |= BIT1;
}

void init(){
    RW&=~BIT1;
    initClock();
    initport();
    write_com(0x38);//8λ���ݽӿڣ�2����ʾ��5*7�ĵ����ַ�
    write_com(0x0c);//��ʾ�������أ���˸��
    write_com(0x06);//���ݶ�д������AC�Զ���1  ���ݶ�д�����󣬻��汣�ֲ���
    write_com(0x01);//����
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    write_com(0x80);
    int len = strlen(buf);
    int i;
    for(i=0;i<len;i++){
        write_com(buf[i]);
        delay_1ms(1);
    }
    return 0;
}

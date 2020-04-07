#include <msp430g2553.h>
#include<string.h>
#include <intrinsics.h>
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define RS P2OUT //2.3
#define RW P2OUT //2.4
#define EN P2OUT //2.5
#define DataPort P1OUT
#define CmdPort P1OUT
unsigned char buf[] = "HELLO WORLD";

void initport(){
    P1SEL&=0X00;                  //����P3��ΪI/Oģʽ
    P2SEL&=0X00;                  //����P6��ΪI/Oģʽ
    P2DIR|=0X38;                  //����P3��Ϊ���ģʽ
    P1DIR|=0XFF;                  //����P6��Ϊ���ģʽ
    P1OUT&=0X00;
    P2OUT&=0X00;
}
void write_cmd(unsigned char cmd){//д�����
    EN&=~BIT5;
    delay_ms(5);
    RS&=~BIT3;
    delay_ms(5);
    RW&=~BIT4;
    CmdPort = cmd;
    delay_ms(5);
    EN|=BIT5;
    delay_ms(5);
    EN&=~BIT5;

   // RW |= BIT4;

}
void write_data(unsigned char data){//д���ݺ���

    RS|=BIT3;
    delay_ms(5);
    EN&=~BIT5;
    RW&=~BIT4;
    delay_ms(5);
    DataPort = data;
    delay_ms(5);
    EN|=BIT5;
    delay_ms(5);
    EN&=~BIT5;
}

void init(){
    RW&=~BIT4;
    initport();
    write_cmd(0x38);//8λ���ݽӿڣ�2����ʾ��5*7�ĵ����ַ�
    write_cmd(0x0c);//��ʾ�������أ���˸��
    write_cmd(0x06);//���ݶ�д������AC�Զ���1  ���ݶ�д�����󣬻��汣�ֲ���
    write_cmd(0x01);//����
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    write_cmd(0xC0);
    int len = strlen(buf);
    int i;
    for(i=0;i<len;i++){
        write_data(buf[i]);
        delay_ms(1);
    }
    return 0;
}

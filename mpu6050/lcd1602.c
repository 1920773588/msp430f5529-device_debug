#include "lcd1602.h"

void initport(){
    P1SEL&=0X00;                  //����P3��ΪI/Oģʽ
    P3SEL&=0X00;                  //����P6��ΪI/Oģʽ
    P1DIR|=0X38;                  //����P3��Ϊ���ģʽ
    P3DIR|=0XFF;                  //����P6��Ϊ���ģʽ
    P1OUT&=0X00;
    P3OUT&=0X00;
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




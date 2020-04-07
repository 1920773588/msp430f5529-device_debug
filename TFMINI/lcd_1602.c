#include "lcd_1602.h"

void initport(){
    P1SEL&=0X00;                  //����P3��ΪI/Oģʽ
    P4SEL&=0Xf0;                  //����P6��ΪI/Oģʽ
    P2SEL&=0x0F;

    P1DIR|=0X38;                  //����P3��Ϊ���ģʽ
    P4DIR|=0X0F;                  //����P6��Ϊ���ģʽ
    P2DIR|=0xF0;
   // P7DIR|= BIT0;

    P1OUT&=~0X38;
    P4OUT&=0Xf0;
    P2OUT&=0X0F;
}
void write_cmd(unsigned char cmd){//д�����
    EN&=~BIT5;
    delay_ms(5);
    RS&=~BIT3;
    delay_ms(5);
    RW&=~BIT4;
    CmdPort_L = cmd;
    CmdPort_H = cmd;
    /*if(cmd & 0x10 == 0)
        CmdPort_H &= ~BIT4;
    else CmdPort_H |= BIT4;
    if(cmd&0x20 == 0)
        P7OUT &= ~BIT0;
    else P7OUT |= BIT0;
    if(cmd & 0x40 == 0)
         CmdPort_H &= ~BIT6;
     else CmdPort_H |= BIT6;
    if(cmd & 0x80 == 0)
         CmdPort_H &= ~BIT7;
     else CmdPort_H |= BIT7;*/
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
    DataPort_L = data;
    DataPort_H = data;
  /*  if(data & 0x10 == 0)
        DataPort_H &= ~BIT4;
    else DataPort_H |= BIT4;
    if(data&0x20 == 0)
        P7OUT &= ~BIT0;
    else P7OUT |= BIT0;
    if(data & 0x40 == 0)
        DataPort_H &= ~BIT6;
     else DataPort_H |= BIT6;
    if(data & 0x80 == 0)
        DataPort_H &= ~BIT7;
     else DataPort_H |= BIT7;*/
    delay_ms(5);
    EN|=BIT5;
    delay_ms(5);
    EN&=~BIT5;
}

void init_lcd1602(){
    RW&=~BIT4;
    initport();
    write_cmd(0x38);//8λ���ݽӿڣ�2����ʾ��5*7�ĵ����ַ�
    write_cmd(0x0c);//��ʾ�������أ���˸��
    write_cmd(0x06);//���ݶ�д������AC�Զ���1  ���ݶ�д�����󣬻��汣�ֲ���
    write_cmd(0x01);//����
}

void display(unsigned char* s,unsigned int index){
    if(index == 1)
        write_cmd(0xc0);
    else
        write_cmd(0x80);
    int len = strlen(s);
    int i=0;
    while(i<len){
        write_data(s[i]);
        i++;
    }
}
void lcd_printf(char *s,unsigned int temp_data)
{
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //ȡ������
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
    *++s =temp_data+0x30;
    *++s=0;
}

void lcd_printf1(char *s,int temp_data)
{
    if(temp_data<0)
    {
        temp_data=-temp_data;
        *s='-';
    }
    else *s='+';
    *++s =temp_data/1000+0x30;
    temp_data=temp_data%1000;     //ȡ������
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //ȡ������
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
    *++s =temp_data+0x30;
    *++s=0;
}


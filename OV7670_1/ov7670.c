/*
 * ov7670.c
 *
 *  Created on: 2019��8��6��
 *      Author: ZZX
 */
#include "ov7670.h"
#include<msp430f5529.h>
#include "SCCB.h"
#include "include.h"
unsigned char ov7670_init(void)
{

  unsigned int i=0;
  unsigned char temp;
  //VSYNC-P2.0
  //�������룬�ⲿ�ж�
  P2DIR &= ~BIT0;
  P2REN |= BIT0;
  P2OUT |= BIT0;//��������

  //FIFO������������
  //D0-D3--P4.0-P4.3,D4-D7--P6.0--P6.3
  //��������
  P4DIR &= 0xf0;
  P4REN |= 0x0f;
  P4OUT |= 0x0f;
  P6DIR &= 0xf0;
  P6REN |= 0x0f;
  P6OUT |= 0x0f;

  //OE-P1.2,RCLK-P1.3,WEN-P1.4,WRST-P1.5,RRST-P1.6
  //���
  P1DIR |= 0xec;
  P1OUT |= 0xec;
  SCCB_init();

     //��д�Ĵ����������ִ���
    if(wr_Sensor_Reg(0x12,0x80)!= 0 ) //Reset SCCB
    {
          return 1;//���󷵻�
    }

        delay_ms(50);

    if(rd_Sensor_Reg(0x0b, &temp) != 0)//��ID
    {
        return 2 ;//���󷵻�
    }

     if(temp==0x73)//OV7670
     {
       for(i=0;i<OV7670_REG_NUM;i++)
       {
        if(wr_Sensor_Reg(OV7670_reg[i][0],OV7670_reg[i][1]) != 0)
        {
                    return 3;//���󷵻�
        }
       }

    }
        return 0; //ok
}

//���ܣ�дOV7660�Ĵ���
//���أ�0-�ɹ�   ����ʧ��
unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat)
{
     startSCCB();//����SCCB ���߿�ʼ��������
    if(0==SCCBwriteByte(0x42))//д��ַ
    {
        stopSCCB();//����SCCB ����ֹͣ��������
        return 1;//���󷵻�
    }
    delay_us(20);
    if(0==SCCBwriteByte(regID))//������ID
    {
        stopSCCB();//����SCCB ����ֹͣ��������
        return 2;//���󷵻�
    }
    delay_us(20);
    if(0==SCCBwriteByte(regDat))//д���ݵ�������
    {
        stopSCCB();//����SCCB ����ֹͣ��������
        return 3;//���󷵻�
    }
    stopSCCB();//����SCCB ����ֹͣ��������

    return 0;//�ɹ�����
}

//���ܣ���OV7660�Ĵ���
//���أ�0-�ɹ�   ����ʧ��
unsigned char rd_Sensor_Reg(unsigned char regID,unsigned char *regDat)
{
    //ͨ��д�������üĴ�����ַ
    startSCCB();
    if(SCCBwriteByte(0x42)==0)//д��ַ
    {

        return 1;//���󷵻�
    }
    delay_us(100);
    if(SCCBwriteByte(regID)==0)//������ID
    {

        return 2;//���󷵻�
    }
        delay_us(100);
    stopSCCB();//����SCCB ����ֹͣ��������

    delay_us(100);

    //���üĴ�����ַ�󣬲��Ƕ�
    startSCCB();
    if(SCCBwriteByte(0x43)==0)//����ַ
    {

        return 3;//���󷵻�
    }
    delay_us(100);
    *regDat=SCCBreadByte();//���ض�����ֵ
    noAck();//����NACK����
    stopSCCB();//����SCCB ����ֹͣ��������
    return 0;//�ɹ�����
}

void OV7670_Window_Set(unsigned int sx,unsigned int sy,unsigned int width,unsigned int height)
{
    unsigned int endx;
    unsigned int endy;
    unsigned char temp;
    endx=(sx+width*2)%784;  //   sx:HSTART endx:HSTOP
    endy=sy+height*2;       //   sy:VSTRT endy:VSTOP

        rd_Sensor_Reg(0x32,&temp);
    temp&=0Xc0;
    temp|=((endx&0X07)<<3)|(sx&0X07);
    wr_Sensor_Reg(0X032,temp);
    wr_Sensor_Reg(0X17,sx>>3);
    wr_Sensor_Reg(0X18,endx>>3);

        rd_Sensor_Reg(0x03,&temp);
    temp&=0Xf0;
    temp|=((endy&0X03)<<2)|(sy&0X03);
    wr_Sensor_Reg(0X03,temp);
    wr_Sensor_Reg(0X19,sy>>2);
    wr_Sensor_Reg(0X1A,endy>>2);
}


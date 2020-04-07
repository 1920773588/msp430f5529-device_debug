/*
 * SCCB.c
 *
 *  Created on: 2019��8��6��
 *      Author: ZZX
 */
#include "SCCB.h"
/*
-----------------------------------------------
   ����: ��ʼ��SCCB�˿ڣ�SCL-P8.1,�����SDA-P8.2,���
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void SCCB_init(void)
{
  //SDA-P8.2,��������
  SCCB_SID_IN;
  P8REN |= BIT2;
  SCCB_SID_H();
  //SCL-P8.1,���
  P8DIR |= BIT1;
  P8OUT |= BIT1;

  SCCB_SID_OUT;
}
/*
-----------------------------------------------
   ����: start����,SCCB����ʼ�ź�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void startSCCB(void)
{
    SCCB_SID_H();     //�����߸ߵ�ƽ
    SCCB_SIC_H();     //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(50);
    SCCB_SID_L();
    delay_us(50);
    SCCB_SIC_L();     //ʱ�ӻָ��͵�ƽ��������������Ҫ
}
/*
-----------------------------------------------
   ����: stop����,SCCB��ֹͣ�ź�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void stopSCCB(void)
{
    SCCB_SID_L();
    delay_us(50);
    SCCB_SIC_H();
    delay_us(50);
    SCCB_SID_H();
    delay_us(50);
}
/*
-----------------------------------------------
   ����: noAck,����������ȡ�е����һ����������
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void noAck(void)
{
    delay_us(50);
    SCCB_SID_H();
    SCCB_SIC_H();
    delay_us(50);
    SCCB_SIC_L();
    delay_us(50);
    SCCB_SID_L();
    delay_us(50);
}
/*
-----------------------------------------------
   ����: д��һ���ֽڵ����ݵ�SCCB
   ����: д������
 ����ֵ: ���ͳɹ�����1������ʧ�ܷ���0
-----------------------------------------------
*/
unsigned int SCCBwriteByte(unsigned int m_data)
{
    unsigned char j,tem;

    for(j=0;j<8;j++) //ѭ��8�η�������
    {
        if(m_data&0x80)
        {
                    SCCB_SID_H();
        }
        else
        {
                    SCCB_SID_L();
        }
                m_data<<=1;
        delay_us(50);
        SCCB_SIC_H();
        delay_us(50);
        SCCB_SIC_L();
    }
    SCCB_SID_IN;/*����SDAΪ����*/
    delay_us(50);
    SCCB_SIC_H();
    delay_us(50);

    if(SCCB_SID_STATE){tem=0;}   //SDA=1����ʧ�ܣ�����0}
    else {tem=1;}   //SDA=0���ͳɹ�������1
    SCCB_SIC_L();
        SCCB_SID_OUT;/*����SDAΪ���*/

    return tem;
}

/*
-----------------------------------------------
   ����: һ���ֽ����ݶ�ȡ���ҷ���
   ����: ��
 ����ֵ: ��ȡ��������
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
    unsigned char read,j;
    read = 0x00;

    SCCB_SID_IN;/*����SDAΪ����*/
    for(j=8;j>0;j--) //ѭ��8�ν�������
    {
        delay_us(50);
        SCCB_SIC_H();
        read=read<<1;
        if(SCCB_SID_STATE)
        {
                    read++;
        }
                delay_us(50);
        SCCB_SIC_L();

    }
        SCCB_SID_OUT;/*����SDAΪ���*/
    return read;
}





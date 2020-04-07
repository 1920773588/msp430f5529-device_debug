#ifndef ESP8266_H_
#define ESP8266_H_

#include <msp430x14x.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

/***************************************************************************
�������ƣ�ESP8266_AT_Init
��        �ܣ���ESP8266��������
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_AT_Init(void);

/***************************************************************************
�������ƣ� ESP8266_AT_RST
��        �ܣ�����ͨ��ģ��
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_AT_RST(void);

/***************************************************************************
�������ƣ�ESP8266_Test_AT_OK
��        �ܣ���ESP8266ģ�鷢��AT
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_Test_AT_OK(void);

/***************************************************************************
�������ƣ�ESP8266_Set_CIPMUX();
��        �ܣ����ö�����ģʽ��CIPMUX=1
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_Set_CIPMUX(void);

/***************************************************************************
�������ƣ�ESP8266_Set_Server_Port()
��        �ܣ����÷������Ķ˿ڣ�8899
��        ������
��  ��  ֵ��OK?
****************************************************************************/
void ESP8266_Set_Server_Port(void);






#endif /*ESP8266_H_*/
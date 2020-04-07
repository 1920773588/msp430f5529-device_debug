#include "esp8266.h"
#include "uart.h"
//

/***************************************************************************
�������ƣ� ESP8266_AT_Init
��        �ܣ���ESP8266�������ã�Ϊȷ����ESP8266���ӣ����ж������
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_AT_Init(void)
{
    unsigned short try_num = 0;
    //
    ESP8266_AT_RST();   //ÿ�ο���������

    for(try_num=0; try_num<3; try_num++)
    {
        ESP8266_Test_AT_OK();
        ESP8266_Set_CIPMUX();
        ESP8266_Set_Server_Port();
    }
}


/***************************************************************************
�������ƣ� ESP8266_AT_RST
��        �ܣ���ESP8266��������
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_AT_RST(void)
{
    Mcu_Uart0SendString("AT+RST\r\n");      //��PC��ӡlog
    Mcu_Uart1SendString("AT+RST\r\n");      //��ESP8266����ָ��
    //
    delay_ms(500);                          //500ms��ʱ

}

/***************************************************************************
�������ƣ�ESP8266_Test_AT_OK
��        �ܣ���ESP8266ģ�鷢��AT,�Ƿ񷵻�OK
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_Test_AT_OK(void)
{
    Mcu_Uart0SendString("AT\r\n");    //��PC��ӡlog
    Mcu_Uart1SendString("AT\r\n");    //��ESP8266����ָ��
    //
    delay_ms(500);                    //500ms��ʱ
}

/***************************************************************************
�������ƣ�ESP8266_Set_CIPMUX();
��        �ܣ����ö�����ģʽ
��        ������
��  ��  ֵ��
****************************************************************************/
void ESP8266_Set_CIPMUX(void)
{
    Mcu_Uart0SendString("AT+CIPMUX=1\r\n");    //��PC��ӡlog
    Mcu_Uart1SendString("AT+CIPMUX=1\r\n");    //��ESP8266����ָ��

    delay_ms(500);                     //500ms��ʱ
}

/***************************************************************************
�������ƣ�ESP8266_Set_Server_Port()
��        �ܣ����÷������Ķ˿ڣ�8899
��        ������
��  ��  ֵ��OK?
****************************************************************************/
void ESP8266_Set_Server_Port(void)
{
    Mcu_Uart0SendString("AT+CIPSERVER=1,8899\r\n");        //��PC��ӡlog
    Mcu_Uart1SendString("AT+CIPSERVER=1,8899\r\n");        //��ESP8266����ָ��

    delay_ms(500);                        //500ms��ʱ
}






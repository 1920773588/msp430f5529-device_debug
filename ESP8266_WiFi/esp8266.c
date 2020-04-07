#include "esp8266.h"
#include "uart.h"
//

/***************************************************************************
函数名称： ESP8266_AT_Init
功        能：对ESP8266进行设置，为确保与ESP8266连接，进行多次设置
参        数：无
返  回  值：
****************************************************************************/
void ESP8266_AT_Init(void)
{
    unsigned short try_num = 0;
    //
    ESP8266_AT_RST();   //每次开机便重启

    for(try_num=0; try_num<3; try_num++)
    {
        ESP8266_Test_AT_OK();
        ESP8266_Set_CIPMUX();
        ESP8266_Set_Server_Port();
    }
}


/***************************************************************************
函数名称： ESP8266_AT_RST
功        能：对ESP8266进行设置
参        数：无
返  回  值：
****************************************************************************/
void ESP8266_AT_RST(void)
{
    Mcu_Uart0SendString("AT+RST\r\n");      //向PC打印log
    Mcu_Uart1SendString("AT+RST\r\n");      //向ESP8266发送指令
    //
    delay_ms(500);                          //500ms延时

}

/***************************************************************************
函数名称：ESP8266_Test_AT_OK
功        能：向ESP8266模块发送AT,是否返回OK
参        数：无
返  回  值：
****************************************************************************/
void ESP8266_Test_AT_OK(void)
{
    Mcu_Uart0SendString("AT\r\n");    //向PC打印log
    Mcu_Uart1SendString("AT\r\n");    //向ESP8266发送指令
    //
    delay_ms(500);                    //500ms延时
}

/***************************************************************************
函数名称：ESP8266_Set_CIPMUX();
功        能：设置多连接模式
参        数：无
返  回  值：
****************************************************************************/
void ESP8266_Set_CIPMUX(void)
{
    Mcu_Uart0SendString("AT+CIPMUX=1\r\n");    //向PC打印log
    Mcu_Uart1SendString("AT+CIPMUX=1\r\n");    //向ESP8266发送指令

    delay_ms(500);                     //500ms延时
}

/***************************************************************************
函数名称：ESP8266_Set_Server_Port()
功        能：设置服务器的端口，8899
参        数：无
返  回  值：OK?
****************************************************************************/
void ESP8266_Set_Server_Port(void)
{
    Mcu_Uart0SendString("AT+CIPSERVER=1,8899\r\n");        //向PC打印log
    Mcu_Uart1SendString("AT+CIPSERVER=1,8899\r\n");        //向ESP8266发送指令

    delay_ms(500);                        //500ms延时
}







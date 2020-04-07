#ifndef UART_H_
#define UART_H_

#include  <msp430x14x.h>
/*******************************************************************************
函数名称：UartInit
功    能：初始化Uart端口,包括UART0和UART1
参    数：无
返回值  ：无
********************************************************************************/
void Mcu_UartInit(void);

/***************************************************************************
函数名称：Mcu_Uart0SendString
功    能：向PC机发送字符串
参    数：无
返回值  ：无
****************************************************************************/
void Mcu_Uart0SendString(unsigned char *ptr);

/***************************************************************************
函数名称：Mcu_Uart1SendString
功    能：向ESP8266模块发送字符串
参    数：无
返回值  ：无
****************************************************************************/
void Mcu_Uart1SendString(unsigned char *ptr);

/***************************************************************************
函数名称：Buffer_Uart1RecvClear
功    能：清空接收区缓存
参    数：无
返回值  ：无
****************************************************************************/
void Buffer_Uart1RecvClear(void);

/***************************************************************************
函数名称：TimerA0Init
功    能：精确延时300ms
参    数：无
返回值：无
****************************************************************************/
void Mcu_TimerA0Start(void);


#endif /* UART_H_ */





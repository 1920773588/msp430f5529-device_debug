#include "lcd12864.h"

void write_cmd_data(uchar cmd_data,uchar cmd_or_data)	//cmd_or_data,0��д���1��д����
{
	uint i,j;
	CS_H;
	SID_H;
	SCLK_L;
	for(i=0;i<5;i++)            //д5��1�������ݽ��г�ʼ��
	{
	    SCLK_H;
	    _NOP();_NOP();_NOP();_NOP();
	    SCLK_L;
	    _NOP();_NOP();
	}

    SID_L;		                //дRW(0)����λ��Ч
    _NOP();
    SCLK_H;
    _NOP();_NOP();_NOP();_NOP();
    SCLK_L;

    if(cmd_or_data)SID_H;       //RS(0or1)����ʱΪRS=1,д����1����0
    _NOP();_NOP();_NOP();
    SCLK_H;
    _NOP();_NOP();_NOP();_NOP();
    SCLK_L;
    _NOP();
    SID_L;		                //д0
    _NOP();_NOP();
    SCLK_H;

    for(j=0;j<2;j++)
    {
        for( i=0;i<4;i++)       //д4λ��
        {
            SCLK_L;
            if(cmd_data&BIT7)
                SID_H;
            else SID_L;
            _NOP();_NOP();_NOP();
            SCLK_H;
            cmd_data<<=1;       //ͨ����λ�ķ������������ֵĴ洢
            _NOP();
        }
        SCLK_L;
        SID_L;
        for(i=0;i<4;i++)        //д4��0
        {
            _NOP();
            SCLK_H;
            _NOP();_NOP();_NOP();_NOP();
            SCLK_L;
        }
    }
    CS_L;
}

void set_DDR_addr(uchar x,uchar y)          //�趨DDRAM��ַ��x:������y������
{
    uchar address=0;
    switch(x)
    {
         case 1:address=0x80|(y-1);break;
         case 2:address=0x90|(y-1);break;
         case 3:address=0x88|(y-1);break;
         case 4:address=0x98|(y-1);break;
    }
    write_cmd_data(address,0);
}

void DispStr(uchar x,uchar y,char *ptr)     //��ʾ�ַ�����x��yΪ0ʱ���趨�µ�ַ
{
    if((x!=0)&&(y!=0))
        set_DDR_addr(x,y);
    while(*ptr!='\0')
    {
        write_cmd_data(*ptr,1);
        ptr++;
    }
}

void DispInt(uchar x,uchar y,uint number)       //����ʾС��65536����Ȼ��������ֵΪ��Ȼ��λ����x��yΪ0ʱ���趨�µ�ַ
{												//�������ʾ
  	uint i;
	uint weight=10000;
	set_DDR_addr(x,y);
	for(i=5;(number/weight==0)&&(i>0);i--)
	{
		number%=weight;
		weight/=10;
	}
	if(i)
	{
		while(weight)
		{
			write_cmd_data(number/weight+'0',1);
			number%=weight;
			weight/=10;
		}
	}
	else write_cmd_data('0',1);
	//return i;
}

void disp_int_right(uchar x, uchar y, uint number)      //x,y Ϊ��ʼ��ַ��numberС��65536
														//�Ҷ�����ʾ
{
	uchar i , j;
	uint weight=10000;
	set_DDR_addr(x,y);
	for(i=5;(number/weight==0)&&(i>0);i--)
	{
		number%=weight;
		weight/=10;
	}

	if(i)
	{
		for(j=0;j<6-i;j++)
		{
			write_cmd_data(' ',1);
		}
		while(weight)
		{
			write_cmd_data(number/weight+'0',1);
			number%=weight;
			weight/=10;
		}
	}
	else
	{
		for(j=0;j<5;j++)
		{
			write_cmd_data(' ',1);
		}
		write_cmd_data('0',1);
	}
}

void disp_int_left(uchar x, uchar y, uint number)           //x,y Ϊ��ʼ��ַ����numberС��65536
															//�Ҷ���
{
	uchar i , j;
	uint weight=10000;
	set_DDR_addr(x,y);
	for(i=5;(number/weight==0)&&(i>0);i--)
	{
		number%=weight;
		weight/=10;
	}

	if(i)
	{
		while(weight)
		{
			write_cmd_data(number/weight+'0',1);
			number%=weight;
			weight/=10;
		}
		for( j=0;j<6-i;j++)
		{
			write_cmd_data(' ',1);
		}
	}
	else
	{
		write_cmd_data('0',1);
		for( j=0;j<5;j++)
		{
			write_cmd_data(' ',1);
		}
	}
}


void LCD_Delay_us(uint time)            //��ʱ����,��16MHz����
{
  for(;time>0;time--)
  {
    _NOP(); _NOP(); _NOP();_NOP();
    _NOP(); _NOP(); _NOP();_NOP();
    _NOP(); _NOP(); _NOP();_NOP();
    _NOP(); _NOP();
  }
}

void LCD_Init(void)               //Һ����ʼ��
{
    LCD_Pin_OUT;
    LCD_Delay_us(40000);
    write_cmd_data(0x30,0);         //�����趨������ָ�
    LCD_Delay_us(100);
    write_cmd_data(0x30,0);         //�����趨������ָ�
    LCD_Delay_us(37);
    write_cmd_data(0x0c,0);         //������ʾ��
    LCD_Delay_us(100);
    write_cmd_data(0x01,0);         //����
    LCD_Delay_us(1600);
}

void LCD_Clear(void)               //Һ������
{
    write_cmd_data(0x01,0);
    LCD_Delay_us(1600);
}

void lcd_printf(char *s,int temp_data)
{
	//temp_data/=16384;
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s='+';
    *++s =temp_data/10000+0x30;
    temp_data=temp_data%10000;     //ȡ������
    *++s =temp_data/1000+0x30;
    temp_data=temp_data%1000;     //ȡ������
	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //ȡ������
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //ȡ������
	*++s =temp_data+0x30;
	*++s=' ';
}
void lcd_printf1(char *s,int temp_data)
{
	//temp_data/=64;
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s='+';
	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //ȡ������
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //ȡ������
	*++s =temp_data+0x30;
	*++s=0;
}

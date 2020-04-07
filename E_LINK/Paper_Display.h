#include <msp430.h>

#define nRST_H  P1OUT|=BIT4  //�ⲿ��λ���ţ��͵�ƽ��λ
#define nRST_L  P1OUT&=~BIT4
#define nCS_H   P1OUT|=BIT7   //SPIƬѡ���ţ��͵�ƽ��Ч
#define nCS_L   P1OUT&=~BIT7
#define SDA_H   P2OUT|=BIT2   //
#define SDA_L   P2OUT&=~BIT2
#define SCLK_H   P2OUT|=BIT0   //SPIͨ��SCK���ţ�ͨ��ʱ��
#define SCLK_L   P2OUT&=~BIT0
#define nDC_H   P1OUT|=BIT5    //����/����������ţ��ߵ�ƽ���ݣ��͵�ƽ����
#define nDC_L   P1OUT&=~BIT5
#define nBUSY   P1IN & BIT0 //�ߵ�ƽ��ʾæ

extern volatile unsigned char DisBuffer[250*16];

void SPI4W_WRITECOM(const unsigned char INIT_COM);
void SPI4W_WRITEDATA(const unsigned char INIT_DATA);
//void SPI4W_WRITE(unsigned char INIT_COM,unsigned char INIT_DATA);
void MYRESET(void);
void WRITE_LUT(void);
void INIT_SSD1673(void);
//void DIS_IMG(const unsigned char num);
void FIRST_PICTURE(void);
//void IMAGE_WRITE(const unsigned char patt_num);
void DELAY_100nS(const unsigned int delaytime);
void DELAY_mS(const unsigned int delaytime);
void DELAY_S(const unsigned int delaytime);
void DELAY_M(const unsigned int delaytime);
//void RING(void);
void READBUSY(void);
//void closebump(void);
//void DisplayString(const char* str, unsigned int sx, unsigned int sy);
void enterdeepsleep(void);
void Init_buff(void);
//void SET_EYEONE(void);
void PaperIO_Int(void);
/*void display(const unsigned char *str, //�ַ���
             const unsigned int xsize,  //x����λ��
             const unsigned int ysize,  //y����λ��
             const unsigned int font,   //���� 0,1,2
             const unsigned int size,   //�ֺ� 0,1
             const unsigned int reverse,//���� 0 ������ʾ�� 1 ����
             const unsigned int fresh   //����ˢ��
             );
*/

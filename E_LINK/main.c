#include <msp430.h> 
#include "Paper_Display.h"
/*
#define TimesNewRoman 0
#define Arial 1
#define ArialBlack 2
#define size8 0
#define size16 1
volatile unsigned char DisBuffer[250*16];
*/
/**
 * main.c
 */
void initClock(){
    //��DCOΪ16MHZ
    DCOCTL = CALDCO_16MHZ;//DCO0+DCO1; //DCO = 3
    BCSCTL1 = CALBC1_16MHZ;//RSEL0+RSEL1+RSEL2+RSEL3;  //RSEL = 15
    //��SMCLKΪ8MHZ
    BCSCTL2 &= ~SELS;  //SELS = 0 ѡ��DCOΪʱ��Դ
    BCSCTL2 |=DIVS0;   //SMCLK��ƵΪ2��Ƶ
    //MCLKĬ��ΪDCOΪ16MHZ��ACLKΪĬ��32KHZ
}
int main(void)
{
    int i=180;
        WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
        //P8DIR |= BIT1;
        //P8OUT &=~ BIT1;
        initClock();
        PaperIO_Int();
        INIT_SSD1673();
       // Init_buff();
    //    display("                        ", 42, 0,TimesNewRoman,size8,1,0);
    //   display(" Huatsing Instruments   ", 42, 16,TimesNewRoman,size8,1,0);
    //    display("012abcABC",80, 0,TimesNewRoman,size16,0,0);
      //  display("012abcABC", 0,50,Arial,size8,1,0);
    //    display("012abcABC",80,50,Arial,size16,1,0);
    //    display("012abcABC", 0,90,ArialBlack,size8,1,0);
    //    display("012abcABC",80,90,ArialBlack,size16,1,0);

    //    DIS_IMG(1);
        DIS_IMG(2);
    //    DIS_IMG(3);
        while(1)
        {
        }

}

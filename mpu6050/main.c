/**********************************************************************************************
     �޸���־��ƫ�����д�������ָʾ�����д�������
**********************************************************************************************/
#include <msp430f5529.h>
#include <math.h>
#include "lcd12864.h"
#include "INTRINSICS.H"
#include "lcd1602.h"
#define uchar unsigned char
#define uint  unsigned int
#define CPU_F ((double)16000000)
//#define delayus(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))  //�궨����ʱ����
#define delayms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define pi 3.141592
#define Gry_offset -0.3         // ������Xƫ����
#define Gry_offsety -0.6        // ������Yƫ����
#define Gry_offsetz 1.4         // ������Zƫ����
#define Gyr_Gain 131            // ������250dpsʱ������(dps/digital)
/************�ܽŶ���******************/
#define SCL1 P2OUT |=BIT0
#define SCL0 P2OUT &=~BIT0
#define SCLOUT P2DIR |= BIT0
#define SDA1 P2OUT |=BIT2       //IIC��������
#define SDA0 P2OUT &=~BIT2
#define SDAIN P2DIR &=~BIT2
#define SDAOUT P2DIR |=BIT2
#define SDADATA (P2IN & BIT2)
/************�ܽŶ���******************/
#define SMPLRT_DIV  0x19        //�����ǲ����ʪ�����ֵ��0x07(125Hz)
#define CONFIG   0x1A           //��ͨ�˲�Ƶ�ʪ�����ֵ��0x06(5Hz)
#define GYRO_CONFIG  0x1B       //�������Լ켰������Χ������ֵ��0x18(���Լ쪢2000deg/s)
#define ACCEL_CONFIG 0x1C       //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʪ�����ֵ��0x01(���Լ쪢2G��5Hz)
#define ACCEL_XOUT_H 0x3B       //X����ٶȸ�λ
#define ACCEL_XOUT_L 0x3C       //X����ٶȵ�λ
#define ACCEL_YOUT_H 0x3D       //Y����ٶȸ�λ
#define ACCEL_YOUT_L 0x3E       //Y����ٶȵ�λ
#define ACCEL_ZOUT_H 0x3F       //Z����ٶȸ�λ
#define ACCEL_ZOUT_L 0x40       //Z����ٶȵ�λ
#define TEMP_OUT_H  0x41        //�¶���ؼĴ�����λ
#define TEMP_OUT_L  0x42        //�¶���ؼĴ�����λ
#define GYRO_XOUT_H  0x43       //X����ٶȸ�λ
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45       //Y����ٶȸ�λ
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47       //Z����ٶȸ�λ
#define GYRO_ZOUT_L  0x48
#define PWR_MGMT_1  0x6B        //��Դ��������ֵ��0x00(��������)
#define WHO_AM_I   0x75         //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define SlaveAddress 0xD0       //IICд��ʱ�ĵ�ַ�ֽ����ݪ�+1Ϊ��ȡ  //��ʾ����(-511��512)���ַ�����
char dis[15];
unsigned char dis1[15];
int dis_data;
int ax,ay,az,gx,gy,gz;
int AX,AY,AZ,GX,GY,GZ;          //���������̬
float kalman1,kalman2,kalman3;
float bias=0.05;                //����ƫ�������ɵ������ȴ�С
float biasy=0.05;
float biasz=0.05;
float P_00=0.001,P_01=0,P_10=0,P_11=0;
float Py_00=0.001,Py_01=0,Py_10=0,Py_11=0;
float Pz_00=0.001,Pz_01=0,Pz_10=0,Pz_11=0;
float Q_angle=0.01;             //bilievble for x
float Q_omega=0.0003;           //bilievble foe y
float R_angle=0.01;             //noise variance
long time_now,time_past;
float dt;                       //sampling time
//float angleG=0;
//float Kx_0,Ky_0,Kz_0;
//float omegax,omegay,omegaz;
uint flag_kalman;
/*void delay1(uint z)
    {
    uint t,y;
    for(t=z;t>0;t--)
    for(y=110;y>0;y--);
}*/
void ini_time()
{
    P1SEL|=BIT2+BIT3+BIT4+BIT5;
    P1OUT|=BIT2+BIT3+BIT4+BIT5;
    TA0CCR0 = 512;                              // PWM ����
    TA0CCTL1 = OUTMOD_7;                        // CCR1��λ��λ
    TA0CCR1 = 100;                              // ����75%ռ�ձ�
    TA0CCTL2 = OUTMOD_7;                        // CCR2 ��λ��λ
    TA0CCR2 =100;                               // ����25%ռ�ձ�
    TA0CCTL3 = OUTMOD_7;                        // CCR3 ��λ��λ
    TA0CCR3 =100;                               // ����25%ռ�ձ�
    TA0CCTL4 = OUTMOD_7;                        // CCR4 ��λ��λ
    TA0CCR4 =100;                               // ����25%ռ�ձ�
    TA0CTL = TASSEL_2 + TACLR;                  // SCLK, ��ģʽ

    TB0CTL|=TBCLR+TBSSEL_2 ;                    //����TAIIE���������жϱ���
    TB0CCR0|=1000;
    TB0CCTL0|=CCIE;
    TB0CTL|=MC_1;

    TA0CTL |= MC_1;
}

void count_x(float a1,float a2,float g)         //�������˲�,����Ƕ�   x��y��z�ֱ��yz��xz��xyƽ��ļн�
{
    a1/=16384;
    a2/=16384;
    g/=131;
    float angleA=(atan(a1/a2))*180/pi;          //(atan(a1/(sqrt(a3*a3+a2*a2)))*180/pi);���ݼ��ٶȷ����õ��ĽǶ�(degree)
    float omega =  Gyr_Gain * (g +  Gry_offset);    //���ٶ�
    //angleG = angleG + omega * dt;                 // �Խ��ٶȻ��ֵõ��ĽǶ�(degree)

    kalman1 += (omega - bias) * dt;                 // �������
       P_00 += -(P_10 + P_01) * dt + Q_angle *dt;
       P_01 += -P_11 * dt;
       P_10 += -P_11 * dt;
       P_11 += +Q_omega * dt;                       // ����������Э����

       float K_0 = P_00 / (P_00 + R_angle);
       float K_1 = P_10 / (P_00 + R_angle);

       bias += K_1 * (angleA - kalman1);
       kalman1 += K_0 * (angleA - kalman1);         // �������
       P_00 -= K_0 * P_00;
       P_01 -= K_0 * P_01;
       P_10 -= K_1 * P_00;
       P_11 -= K_1 * P_01;                          // ����������Э����

}
void count_y(float a1,float a2,float g)
{
        a1/=16384;
        a2/=16384;
        g/=131;
        float angleA=(atan(a1/a2))*180/pi;          //(atan(a1/(sqrt(a3*a3+a2*a2)))*180/pi);���ݼ��ٶȷ����õ��ĽǶ�(degree)
        float omega =  Gyr_Gain * (g +  Gry_offsety);       //���ٶ�

        kalman2 += (omega - biasy) * dt;                    // �������
               Py_00 += -(Py_10 + Py_01) * dt + Q_angle *dt;
               Py_01 += -Py_11 * dt;
               Py_10 += -Py_11 * dt;
               Py_11 += +Q_omega * dt;                      // ����������Э����

               float K_0 = Py_00 / (Py_00 + R_angle);
               float K_1 = Py_10 / (Py_00 + R_angle);

               biasy += K_1 * (angleA - kalman2);
               kalman2 += K_0 * (angleA - kalman2);         // �������
               Py_00 -= K_0 * Py_00;
               Py_01 -= K_0 * Py_01;
               Py_10 -= K_1 * Py_00;
               Py_11 -= K_1 * Py_01;                        // ����������Э����

}
void count_z(float a1,float a2,float g)
{
        a1/=16384;
        a2/=16384;
        g/=131;
        float angleA=(atan(a1/a2))*180/pi;          //(atan(a1/(sqrt(a3*a3+a2*a2)))*180/pi);���ݼ��ٶȷ����õ��ĽǶ�(degree)
        float omega =  Gyr_Gain * (g +  Gry_offsetz);       //���ٶ�

        kalman3 += (omega - biasz) * dt;                    // �������
               Pz_00 += -(Pz_10 + Pz_01) * dt + Q_angle *dt;
               Pz_01 += -Pz_11 * dt;
               Pz_10 += -Pz_11 * dt;
               Pz_11 += +Q_omega * dt;                      // ����������Э����

               float K_0 = Pz_00 / (Pz_00 + R_angle);
               float K_1 = Pz_10 / (Pz_00 + R_angle);

               biasz += K_1 * (angleA - kalman3);
               kalman3 += K_0 * (angleA - kalman3);         // �������
               Pz_00 -= K_0 * Pz_00;
               Pz_01 -= K_0 * Pz_01;
               Pz_10 -= K_1 * Pz_00;
               Pz_11 -= K_1 * Pz_01;                        // ����������Э����
}

void delayus(uint time)
{
    for(;time>0;time--)
    {
        _NOP(); _NOP(); _NOP();_NOP();
        _NOP(); _NOP(); _NOP();_NOP();
        _NOP(); _NOP(); _NOP();_NOP();
        _NOP(); _NOP();
    }
}

//**************************************
//I2C��ʼ�ź�
//**************************************
void I2C_Start()
{
    SDAOUT;
    SCLOUT;
    SDA1;                       //����������
    SCL1;                       //����ʱ����
    delayus(5);                 //��ʱ
    SDA0;                       //�����½���
    delayus(5);                 //��ʱ
    SCL0;                       //����ʱ����
}

//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop()
{
    SDAOUT;
    SCLOUT;
    SDA0;                       //����������
    delayus(5);                 //��ʱ
    SCL1;                       //����ʱ����
    delayus(5);                 //��ʱ
    SDA1;                       //����������
}

//**************************************
//I2C����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uchar ack)
{
    SDAOUT;
    SCLOUT;
    //SDA = ack;                //дӦ���ź�
    if(ack) SDA1;
       else SDA0;
    SCL1;                       //����ʱ����
    delayus(5);                 //��ʱ
    SCL0;                       //����ʱ����
    delayus(5);                 //��ʱ
}


//**************************************
//I2C����Ӧ���ź�
//**************************************
uint I2C_RecvACK()
{
    uchar cy;
    SCLOUT;
    SDAOUT;
    SDA1;
    SCL1;                       //����ʱ����
    delayus(2);                 //��ʱ
    SDAIN;
   // while(P2IN & BIT1)
    //    SCL0;

    if(SDADATA)
    {
        cy=1;//???
    }
    else
    {
        cy=0;
    }
    //cy = SDA;                 //��Ӧ���ź�
    SCL0;                       //����ʱ����
    delayus(5);                 //��ʱ
    SDAOUT;
    return cy;
}

//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
    SDAOUT;
    SCLOUT;
    for (i=0; i<8; i++)         //8λ������
    {
        if((dat<<i)&0x80)
        {
            SDA1;
        }
        else
        {
            SDA0;
        }
        SCL1;                   //����ʱ����
        delayus(5);             //��ʱ
        SCL0;                   //����ʱ����
        delayus(5);             //��ʱ
    }
   // SCL0;
  //  delayus(2);
  //  SDA1;
  //  delayus(2);
    I2C_RecvACK();
}

//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat = 0,cy;
    SDAOUT;
    SCLOUT;
    SDA1;                       //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        SDAIN;
        dat <<= 1;
        SCL1;                   //����ʱ����???
        if(SDADATA)
        {
            cy=1;
        }
          else
          {
              cy=0;
          }
        delayus(5);
        dat |= cy;              //������
        SCL0;                   //����ʱ����
        delayus(5);
    }

    return dat;
}


//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(uchar REG_Address,uchar REG_data)
{

    SDAOUT;
    SCLOUT;
    SDA1;
    SCL1;
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
   // I2C_RecvACK();

    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    //I2C_RecvACK();

    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
   // I2C_RecvACK();

    I2C_Stop();                   //����ֹͣ�ź�
   // delayus(1000);
}

//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
uchar Single_ReadI2C(uchar REG_Address)
{
    uchar REG_data;
   /* SDAOUT;
    SCLOUT;
    SDA1;
    SCL1;*/
    I2C_Start();                   //��ʼ�ź�

    I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
   // I2C_RecvACK();

    I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ
   // I2C_RecvACK();

    I2C_Start();                   //��ʼ�ź�
    I2C_SendByte(SlaveAddress|0x01);  //�����豸��ַ+���ź�
   // I2C_RecvACK();

    REG_data=I2C_RecvByte();       //�����Ĵ�������
    I2C_SendACK(1);                //���շ�Ӧ���ź�

    I2C_Stop();                    //ֹͣ�ź�
    return REG_data;
}

//**************************************
//��ʼ��MPU6050
//**************************************

void InitMPU6050()
{
    Single_WriteI2C(PWR_MGMT_1, 0x00); //�������״̬
    Single_WriteI2C(SMPLRT_DIV, 0x07);//�����ǲ����ʣ�����ֵ��0x07(125Hz)
    Single_WriteI2C(CONFIG, 0x06);//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
    Single_WriteI2C(GYRO_CONFIG, 0x18); //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
    Single_WriteI2C(ACCEL_CONFIG, 0x01);//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
}


//**************************************
//�ϳ�����
//**************************************

unsigned int GetData(uchar REG_Address)
{
    char H,L;
    H=Single_ReadI2C(REG_Address);
    L=Single_ReadI2C(REG_Address+1);
    unsigned int data = (H<<8)|L;
    return data;                //�ϳ�����
}
void main(void)
{
     WDTCTL = WDTPW+WDTHOLD;        // Stop watchdog timer
     P5SEL |= BIT2+BIT3;
     UCSCTL6 &= ~XT2OFF;            //��XT2
    /*********************�Ĵ������ò���******************************/
     __bis_SR_register(SCG0);
     UCSCTL0 = DCO0+DCO1+DCO2+DCO3+DCO4;
     UCSCTL1 = DCORSEL_4;                    //DCOƵ�ʷ�Χ��28.2MHZ����
     UCSCTL2 = FLLD_4 + 1;                   //D=16��N=1
     UCSCTL3 = SELREF_5 + FLLREFDIV_3;       //n=8,FLLREFCLKʱ��ԴΪXT2CLK��DCOCLK=D*(N+1)*(FLLREFCLK/n);DCOCLKDIV=(N+1)*(FLLREFCLK/n);
     UCSCTL4 = SELA_4 + SELS_3 +SELM_3;      //ACLK��ʱ��ԴΪDCOCLKDIV,MCLK\SMCLK��ʱ��ԴΪDCOCLK
     UCSCTL5 = DIVA_5 +DIVS_1;               //ACLK��DCOCLKDIV��32��Ƶ�õ���SMCLK��DCOCLK��2��Ƶ�õ�
                                             //����MCLK:16MHZ,SMCLK:8MHZ,ACLK:32KHZ

     __bic_SR_register(SCG0);                //Enable the FLL control loop
    /**********************************************************************/
     //__delay_cycles(8192);
     do
     {
         UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);    //Clear XT2,XT1,DCO fault flags
         SFRIFG1 &= ~OFIFG;                             //Clear fault flags
     }while (SFRIFG1&OFIFG);

       //  P1DIR |= BIT0;                                 // LED1 ���
        // LCD_Init();
         InitMPU6050();
         ini_time();
         init();
         _BIS_SR(GIE);                                  //���ж�
     int j=0;
     while(1)
     {
     //P1OUT ^=BIT0;
     ax=(int)(GetData(ACCEL_XOUT_H));//16384);
     ay=(int)(GetData(ACCEL_YOUT_H));//16384);
     az=(int)(GetData(ACCEL_ZOUT_H));//16384);
     gx=(int)(GetData(GYRO_XOUT_H)/32);
     gy=(int)(GetData(GYRO_YOUT_H)/32);
     gz=(int)(GetData(GYRO_ZOUT_H)/32);
     /*AX=ax/16384;
     AY=ay/16384;
     AZ=az/16384;
     GX=gx/131;
     GY=gy/131;
     GZ=gz/131;*/

     /*
     if(flag_kalman>=1)
     {
     count_x(ay,az,gx);                 //ʵ�ʶ�Ӧ�������̵�x�ᣬ���ո���ָʾ����ת�������
     count_y(az,ax,gy);                 //ʵ�ʶ�Ӧ�������̵�y�᣿
     count_z(ax,ay,gz);                 //ʵ�ʶ�Ӧ�������̵�z��
     lcd_printf1(dis,kalman1);
     //DispStr(4,1,dis);
     delayus(50);
     lcd_printf1(dis,kalman2);
    // DispStr(4,4,dis);
     delayus(50);
     lcd_printf1(dis,kalman3);
    // DispStr(4,7,dis);
     delayus(50);
     lcd_printf1(dis,angleG);
     //DispInt(3,7,angleG);
     //delayus(50);
     }*/

     lcd_printf(dis,ax);
     //DispStr(1,1,dis);                  //��ʾX����ٶ�
    // delayus(50);
     lcd_printf(dis+7,ay);
    // DispStr(1,4,dis);                  //��ʾY����ٶ�
     //delayus(50);
     //lcd_printf(dis+10,az);
    // DispStr(2,1,dis);                  //��ʾZ����ٶ�
     //delayus(50);
     lcd_printf(dis1,az);
    // DispStr(2,4,dis);                  //��ʾX����ٶ�
     //delayus(50);
     lcd_printf(dis1+7,gy);
    // DispStr(3,1,dis);                  //��ʾY����ٶ�
     //delayus(50);
     //lcd_printf(dis1+10,gz);
    // DispStr(3,4,dis);
     //delayms(30);
     write_cmd(0x01);
     write_cmd(0x80);
     dis[14]=0x30+j;
     if(j==9){
         j=0;
     }else
         j++;
     int i=0;
     while(i<15){
         write_data(dis[i]);
         delay_ms(1);
         i++;
     }
     write_cmd(0xC0);
     i=0;
     while(i<14){
         write_data(dis1[i]);
         delay_ms(1);
         i++;
     }
     delay_ms(1000);
     if(flag_kalman>1000)               //��������
       {
          bias=0.05;                    //define bias index
          biasy=0.05;
          biasz=0.05;
          P_00=0.001,P_01=0,P_10=0,P_11=0;
          Py_00=0.001,Py_01=0,Py_10=0,Py_11=0;
          Pz_00=0.001,Pz_01=0,Pz_10=0,Pz_11=0;
          flag_kalman=0;
       }
     }
}
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
{      dt=TB0R/1000;
       flag_kalman++;
       TB0R=0;
       P1OUT ^= BIT0;                   //LED1 ��˸
       TB0CCR0 = 1000;                  //CCR0 ��������
       TB0CTL&=~TBIFG;
}

#include <msp430g2553.h>
#include <math.h>
#include "INTRINSICS.H"
#include "lcd1602.h"
#define uchar unsigned char
#define uint  unsigned int
//#define CPU_F ((double)16000000)
//#define delayus(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))  //�궨����ʱ����
//#define delayms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define pi 3.141592
#define Gry_offset -0.3         // ������Xƫ����
#define Gry_offsety -0.6        // ������Yƫ����
#define Gry_offsetz 1.4         // ������Zƫ����
#define Gyr_Gain 131            // ������250dpsʱ������(dps/digital)
/************�ܽŶ���******************/
#define SCL1 P2OUT |=BIT0
#define SCL0 P2OUT &=~BIT0
#define SCLOUT P2DIR |= BIT0
#define SDA1 P2OUT |=BIT1       //IIC��������
#define SDA0 P2OUT &=~BIT1
#define SDAIN P2DIR &=~BIT1
#define SDAOUT P2DIR |=BIT1
#define SDADATA (P2IN & BIT1)
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
uint flag_kalman;


void delayus(uint z)
{
    uint i,j;
      for(i = 0;i<20;i++)
        for(j=0;j<z;j++)
        {;;};
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
     InitMPU6050();
     _BIS_SR(GIE);                                  //���ж�
     init();
     int j=0;
     while(1)
     {
     ax=(int)(GetData(ACCEL_XOUT_H)/16384);
     ay=(int)(GetData(ACCEL_YOUT_H)/16384);
     az=(int)(GetData(ACCEL_ZOUT_H)/16384);
     gx=(int)(GetData(GYRO_XOUT_H));//32);
     gy=(int)(GetData(GYRO_YOUT_H));//32);
     gz=(int)(GetData(GYRO_ZOUT_H));//32);

     lcd_printf1(dis,ax);

     lcd_printf1(dis+7,ay);

     dis[14]=0x30+j;
     if(j==9){
         j=0;
     }else
         j++;
    // lcd_printf(dis+10,az);

     lcd_printf1(dis1,az);

     lcd_printf1(dis1+7,gz);

     //lcd_printf(dis1+10,gz);
     write_cmd(0x01);//����

     write_cmd(0x80);
     int i=0;
     while(i<15){
         write_data(dis[i]);
         delayus(1);
         i++;
     }
     write_cmd(0xC0);
     i=0;
     while(i<13){
         write_data(dis1[i]);
         delayus(1);
         i++;
     }
   //  delay_ms(10);
     }
}

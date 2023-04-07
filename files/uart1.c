#include <C8051F340.H>   
#include <intrins.h>   
#include "lcd.h"   
   
#define BAUD_RATE    9600                           //������   
#define SYSCLK       12000000                       //ϵͳʱ��Ƶ��   
   
#define Clms P4   
sbit LED= P3^7;   
sbit RFIN=P1^7;   
sbit INT_0 = P0^2;   
sbit INT_1 = P0^3;   
   
sbit T_CS = P1^6;   
sbit T_BUSY = P1^5;   
   
unsigned char flag;   
unsigned char Buf1Temp;   
unsigned int count1ms=0;   
unsigned long Data_X_Y;   
unsigned long Data_X1_Y;   
unsigned long Data_X2_Y;   
unsigned long Data_X3_Y;   
   
void command_send (unsigned char *str, unsigned short int strlen);   
void delayms(unsigned int m);   
void Sound(void);   
   
unsigned long RdTData();   
   
/****************************************************************  
                ��ʱ��0  
****************************************************************/   
void Timer0_Init (void)   
{   
    CKCON|=0x04;                         //ʹ��ϵͳʱ��   
    TMOD|=0x01;                          //16Bit   
    TR0 = 0;                             // ֹͣ��ʱ��0   
    TH0 = (-SYSCLK/1000) >> 8;           // ��ʱ��1ms���   
    TL0 = -SYSCLK/1000;   
    TR0 = 1;                             // ������ʱ��                             
    ET0 = 1;                             //��ʱ��0�ж�����   
}   
   
void Timer0_ISR (void) interrupt INTERRUPT_TIMER0       //1ms   
{   
    EA = 0;   
    TH0 = (-SYSCLK/1000) >> 8;     
    TL0 = -SYSCLK/1000;   
    if(count1ms)   count1ms--;   
    EA = 1;   
}   
   
/****************************************************************  
                 UART1  
*****************************************************************/   
void UART1_Init (void)   
{   
    SBCON1 |=0x43;   
    SMOD1  =0x6C;    
    SBRLH1 = (-SYSCLK/2/BAUD_RATE)>>8;   
    SBRLL1 = (-SYSCLK/2/BAUD_RATE);    
    SCON1  =0x10;                               
    EIE2   |=0x02;                          
}   
   
void UART1_ISR(void) interrupt INTERRUPT_UART1   
{   
    EA = 0;   
    if((SCON1 & 0x01)==0x01)   
    {      
        Buf1Temp = SBUF1;         // ��SBUF1����������ձ�־λ   
        SCON1 &= ~0x01;                             
    }   
    EA = 1;    
}   
   
/*****************************************************************  
                �˿ڳ�ʼ������  
*****************************************************************/   
void Port_Init(void)   
{   
    XBR0    = 0x07;    //SMB SPI ����0 ������                  
    XBR1    = 0x40;            
    XBR2    = 0x01;                  
    P0SKIP  = 0xCC;  //P0������   11001100   
    P1SKIP  = 0x00;  //P1�ڲ�����              
    SPI0CN  &=0xf3;  //SPI������Ϊ3�ߵ�����ʽ   
    P0MDIN  &=0x3F;       
    P1MDOUT = 0xFF;   //P1���������   
    P1MDIN  = 0xFF;   
    P2MDOUT = 0xFF;                 
    P3MDOUT = 0xFF;   
    P4MDOUT |=0xFC;                          
}   
   
/******************************************************************  
                      ����ɨ��  
******************************************************************/   
void scan_key(void)   
{   
    unsigned char ScanClm ;   
    Clms = 0x0F;   
    ScanClm = Clms;   
    ScanClm = ScanClm & 0x03;                                                  
    if(ScanClm!=0x03)   
      {   
         switch(ScanClm)   
           {   
              case 0x01: Sound();command_send("0x01",4);break;   
              case 0x02: Sound();command_send("0x02",4);break;   
              default: command_send("0x00",4);break;   
            }    
      }   
    else{       
           Clms = 0x17;   
           ScanClm = Clms;   
           ScanClm = ScanClm & 0x03;   
           if(ScanClm!=0x03)   
              {   
                 switch(ScanClm)   
                   {   
                     case 0x01: Sound();command_send("0x03",4);break;   
                     case 0x02: Sound();command_send("0x04",4);break;   
                     default: command_send("0x00",4);break;   
                    }    
              }   
           else {   
                   Clms = 0x1B;   
                   ScanClm = Clms;   
                   ScanClm = ScanClm & 0x03;   
                   if(ScanClm!=0x03)   
                      {   
                        switch(ScanClm)   
                         {    
                           case 0x01: Sound();command_send("0x05",4);break;   
                           case 0x02: Sound();command_send("0x06",4);break;   
                           default: command_send("0x00",4);break;   
                         }    
                       }   
                    else command_send("0x00",4);   
                 }          
         }   
                                    
}   
   
/****************************************************************  
                  �ⲿ�ж�0  
****************************************************************/   
void INT0_Init(void)   
{   
    IT01CF = 0x32;   
    TCON |=0x01;   
    EX0 =1;   
}   
   
void INT0_ISR(void) interrupt INTERRUPT_INT0   
{   
    EA=0;   
    if(INT_0==0)   
       {   
            delayms(5);          //����ʼ����   
            if(INT_0==0)   
              {   
                scan_key();   
                while(INT_0==0);     //�ȴ������ſ�   
                delayms(30);         //���ſ�����   
                while(INT_0==0);     //������ſ��ˣ�   
              }    
                
       }          
    EA=1;   
}   
   
/****************************************************************  
                  �ⲿ�ж�1  
****************************************************************/   
void INT1_Init(void)   
{   
    IT01CF = 0x32;    //�ܽŷ���   
    TCON |=0x04;      //���ش���   
    EX1 =1;        //�ж�����   
}   
   
void INT1_ISR(void) interrupt INTERRUPT_INT1   
{   
    EA=0;   
    RFIN=~RFIN;   
    Data_X_Y=RdTData();   
    Data_X1_Y=RdTData();   
    flag=1;   
    while(INT_1==0);   
    EA=1;   
}   
   
/***************************************************************  
                SPI��ʼ��  
***************************************************************/   
void SPI_Init(void)   
{   
    SPI0CFG |=0x70;   
    SPI0CKR =0x02;   
    ESPI0 =0;   
    SPIEN =1;   
}   
   
void InitTP(unsigned char Command)   
{   
    T_CS=0;   
    SPI0DAT=Command;   
    while(!SPIF);   
    SPIF=0;   
    T_CS=1;   
}   
   
unsigned int TdData(unsigned char Command)   
{   
    unsigned int Tdata=0;   
    T_CS=0;   
    delayms(10);   
    SPI0DAT=Command;   
    while(!SPIF);   
    SPIF=0;   
   
//  INT_1=1;   
//  delayms(1);   
       
    T_BUSY=1;   
    delayms(1);   
    while(T_BUSY);   
   
    SPI0DAT=0x80;   
    while(!SPIF);   
    SPIF=0;   
    Tdata=SPI0DAT;   
    INT_1=1;   
    T_CS=1;   
    return Tdata;   
}   
   
unsigned long RdTData(void)   
{   
    unsigned int txdata,tydata,txcomp=0,tycomp=0;   
    unsigned long tvalue=0;   
    unsigned char i;   
    do   
    {   
        LED=1;   
        txdata=TdData(0x9b);   
        InitTP(0x9b);   
        tydata=TdData(0xdb);   
        InitTP(0x9b);   
        i=1;   
        if((txdata-txcomp)>1|(tydata-tycomp)>1)   
        {   
            txcomp=txdata;   
            tycomp=tydata;   
            delayms(5);   
            i=1;   
        }   
    }   
    while(i==0);   
    tvalue=txdata;   
    tvalue=tvalue<<8;   
    tvalue=tvalue+tydata;   
    return tvalue;   
}   
   
/*****************************************************************  
                      ϵͳ��ʼ��  
*****************************************************************/   
   
void System_Init(void)   
{   
    EA=0;   
    PCA0MD &=~0x40; //�رտ��Ź���ʱ��   
    OSCICN |= 0x03;   
    Port_Init();   
    Timer0_Init();   
    UART1_Init ();   
    INT0_Init();   
    INT1_Init();   
    SPI_Init();   
    EA=1;   
}   
   
/*****************************************************************  
                      �����ַ���  
*****************************************************************/   
void command_send (unsigned char *str, unsigned short int strlen)   
{   
    unsigned char k = 0;   
    do   
    {   
       SBUF1 = *(str + k);    
       while((SCON1 & 0x02)!=0x02);           
       SCON1 &= ~0x02;               
       k ++;   
    }   
    while (k < strlen);   
}   
   
void Sendchar(unsigned char chr)   
{   
    SBUF1 = chr;    
    while((SCON1 & 0x02)!=0x02);           
    SCON1 &= ~0x02;   
}   
   
/*******************************************************************  
                       ��ʱ1ms*T  
*******************************************************************/   
void Delay1ms(unsigned int T)   
{   
    count1ms=T;   
    while(count1ms);   
}   
   
void delayms(unsigned int m)   
{   
    unsigned int j;   
    unsigned int i;    
    for(i=0; i<m; i++)   
     {     
       for(j=0; j<100; j++);   
      }   
}   
   
void Sound(void)   
{   
    unsigned char i;   
    for(i=0;i<100;i++)   
    {   
        Clms = 0x03;   
        delayms(2);   
        Clms = 0xE3;   
        delayms(2);   
    }   
}   
   
void main()   
{          
    System_Init();  //��ʼ����Ƭ����Һ��   
    LED=0;delayms(1000);   
    LED=1;delayms(1000);   
    Sendchar(0x00);   
    Sendchar(Data_X_Y);   
    LED=0;   
    while(1)   
    {   
        Clms |=0x40;   
        delayms(1000);   
        Clms &=0xbf;   
        delayms(1000);   
        InitTP(0x98);   
        if(flag)   
        {   
            flag=0;   
            Sendchar(Data_X_Y>>8);   
            Sendchar(Data_X_Y);   
            Sendchar(Data_X1_Y>>8);   
            Sendchar(Data_X1_Y);   
        }   
    }   
}  

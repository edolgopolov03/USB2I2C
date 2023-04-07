#include "c8051F340.h"
//#include "volkov.h"
#include "uart.h"
#include "USB_Main.h"
#include <stdio.h>


void InitUart(void)
{
  SCON0 = 0x10;                        // 8-bit variable bit rate and RX enabled
  if (SYSCLK/BAUDRATE/2/256 < 1)
  {
    TH1    = -(SYSCLK/BAUDRATE/2);
    CKCON |= 0x08;                     // Timer1 uses SYSCLK
  } 
  else if (SYSCLK/BAUDRATE/2/256 < 4)
  {
    TH1    = -(SYSCLK/BAUDRATE/2/4);
    CKCON &= 0xF0;                     // Timer1 uses SYSCLK/4
    CKCON |= 0x01;
  }
  else if (SYSCLK/BAUDRATE/2/256 < 12)
  {
    TH1    = -(SYSCLK/BAUDRATE/2/12);
    CKCON &= 0xF0;                     // Timer1 uses SYSCLK/12
  }
  else
  {
    TH1    = -(SYSCLK/BAUDRATE/2/48);
    CKCON &= 0xF0;                     // Timer1 uses SYSCLK/48
    CKCON |= 0x03;
  }
  TL1   = 0xFF;                        // set Timer1 to overflow immediately
  TMOD &= 0x0F;                        // Timer1 in 8-bit autoreload
  TMOD |= 0x20;
  TR1   = 1;                           // start Timer1
  TI0   = 1;                           // indicate TX0 ready


//  InitVolkov(0);
//  ES0   = 1;                           // Enable UART0 interrupt
}


void SendByte(unsigned char nch, unsigned char value) 
{  
	SBUF0 = value;
	printf("Byte 0x%02x is sended.\n", (int)value);
}

void RecvByte(unsigned char nch, unsigned char *value) 
{ 
	*value=SBUF0;
}
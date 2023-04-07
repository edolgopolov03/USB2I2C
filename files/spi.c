//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <c8051F340.h>                 // SFR declarations
//#include "global.h"                    // Global CONSTANTS

sbit SS = P0^3;
//sbit CS = P2^5;

//-----------------------------------------------------------------------------
// SPI_Init
//-----------------------------------------------------------------------------
void SPI_Init(void)
{
  SS=1;
//  ESPI0 = 1;
  SPI0CFG = 0x40;
  SPI0CN  = 0x01;
  SPI0CKR = 0x12; // SYSCLK = 48 MHz -> f_clk = 48 / 2 / (SPI0CKR + 1) = 6 MHz; //0x79;
  SPIF = 0;
}

void SPI_MasterTransmit(unsigned char cData)
{
  SPIF = 0;
  SPI0DAT = cData;
  while(!SPIF);
}

unsigned char SPI_MasterReceive(void)
{
//while(!SPIF);
  return SPI0DAT;
}

unsigned short SPI_MasterSend(unsigned char byte1, unsigned char byte2)
{
 unsigned char dlo, dhi;
 SS=0;
 SPI_MasterTransmit(byte1);
 dhi=SPI_MasterReceive();
 SPI_MasterTransmit(byte2);
 dlo=SPI_MasterReceive();
 SS=1;
 return (((unsigned short)dlo)|((unsigned short)dhi<<8));
}

//-----------------------------------------------------------------------------
// SPI_ISR: SPI Interrupt Service Routine
//-----------------------------------------------------------------------------
/*void SPI_ISR (void) interrupt INTERRUPT_SPI0
{
  SPIF = 0;
}
*/
#include <stdio.h>
#include <c8051F340.h>                 // SFR declarations
#include "uart.h"
#include "volkov.h"

unsigned char AddressPLC[CHNUM] =  {0,0};
unsigned char volkov_flag[CHNUM] = {0,0};
//unsigned char echo_flag[CHNUM] = {0,0};
unsigned char echo_flag[CHNUM] = {1,1};
unsigned char flag32[CHNUM] = {0,0};
unsigned char ma_sl_flag[CHNUM] = {1,1};//master/slave flag 1=master 0=slave

unsigned char tbuf[CHNUM][TBUF_SIZE];
unsigned char rbuf[CHNUM][RBUF_SIZE];

unsigned long t_in[CHNUM] = {0,0};
unsigned long t_out[CHNUM] = {0,0};

unsigned long r_in[CHNUM] = {0,0};

unsigned long r_out[CHNUM] = {0,0};


unsigned char ti_restart[CHNUM] = {0,0};

unsigned char START_flag[CHNUM] = {0,0};

unsigned char STOP_echo_flag[CHNUM] = {0,0};

unsigned char STOP_flag[CHNUM] = {0,0};

void VolkovEnable(unsigned char nch)
{
 volkov_flag[nch]=1;
}

void VolkovDisable(unsigned char nch)
{
 volkov_flag[nch]=0;
}

void InitVolkov(unsigned char nch)
{
 t_in[nch] = 0;
 t_out[nch] = 0;
 r_in[nch] = 0;
 r_out[nch] = 0;
 START_flag[nch] = 0;
 STOP_flag[nch] = 0;
 ti_restart[nch] = 1;
// VolkovEnable(nch);} 
}

void RX_irq(unsigned char nch)
{
  unsigned char temp;
  RecvByte(nch, &temp);
//  printf("%02d: %02x\n", r_in[nch], temp);
  if (volkov_flag[nch]==0)
  {
   rbuf[nch][r_in[nch] & (RBUF_SIZE-1)] = temp; //укладываем байты
   r_in[nch]++;
  }
  else
  {   //если установлен режим с эхо и это слейв, то отправляем эхо
   if ((ma_sl_flag[nch] == 0) && (echo_flag[nch] == 1) && (ti_restart[nch] == 1)) SendByte(nch, temp);
   if (temp == START)
   {
    r_in[nch] = 0; 
    r_out[nch] = 0; 
    START_flag[nch] = 1; 
    STOP_echo_flag[nch] = 0;
    STOP_flag[nch] = 0;
   }
   if (START_flag[nch] == 1) //пока старт не придёт, ничего не делать 
   {
    if (((r_in[nch] - r_out[nch]) & ~(RBUF_SIZE-1)) == 0)
    {
     rbuf[nch][r_in[nch] & (RBUF_SIZE-1)] = temp; //укладываем байты
     r_in[nch]++;
    }
    if (temp==STOP) //уложенный байт мог оказаться последним
    {
     if ((ma_sl_flag[nch]==1) && (echo_flag[nch]==1)) //если контроллер в режиме мастера
     {                                      //проверяем режим работы с эхо
       if (STOP_echo_flag[nch]==1) 
       {
        START_flag[nch]=0;
        STOP_echo_flag[nch]=0;
        STOP_flag[nch]=1;
       }
       else
       {
        r_in[nch]=0;
        r_out[nch]=0;
        STOP_echo_flag[nch]=1;
       }
     } 
     else //если режим работы без эхо
     {
      START_flag[nch] = 0; //просто конец приёма
      STOP_flag[nch] = 1;
     } 
    }
   } 
  }
}

void TX_irq(unsigned char nch)
{
  if (t_in[nch] != t_out[nch])
  {
   SendByte(nch,tbuf[nch][t_out[nch] & (TBUF_SIZE-1)]);
   t_out[nch]++;
  }
  else
  {
   t_in[nch]=0;							// empty transmit buffer
   t_out[nch]=0;
   ti_restart[nch]=1;
  }  
}

void wait_end_of_send(unsigned char nch)
{
  ti_restart[nch]=0;
  TI0=1;
  while(ti_restart[nch]==0);
}

void put_char(unsigned char nch, unsigned char dat)
{
	if ((dat == START) || (dat == STOP) || (dat == SHIFT))
	{
		tbuf[nch][t_in[nch]++] = SHIFT;
		if (t_in[nch] >= TBUF_SIZE) wait_end_of_send(nch);
		tbuf[nch][t_in[nch]++] = dat - START;
		if (t_in[nch] >= TBUF_SIZE) wait_end_of_send(nch);
	}
	else 
	{
		tbuf[nch][t_in[nch]++] = dat;
		if (t_in[nch] >= TBUF_SIZE) wait_end_of_send(nch);
	}
}

void BeginSend(unsigned char nch)
{
     ti_restart[nch]=0;
     t_in[nch]=0;									// empty transmit buffer
     t_out[nch]=0;
     START_flag[nch] = 0;
     STOP_echo_flag[nch]=0;
     STOP_flag[nch] = 0;
     if (ma_sl_flag[nch]==1) //Для серверной версии посылка идёт со стартом
     {   
	tbuf[nch][t_in[nch]++]=START; 
        wait_end_of_send(nch);
     }
}

void FinishSend(unsigned char nch)
{
    tbuf[nch][t_in[nch]++] = STOP;
    wait_end_of_send(nch);
 if ((ma_sl_flag[nch] == 1) && (echo_flag[nch] == 0))
 {
//Для серверной версии
   r_in[nch] = 0; 
   r_out[nch] = 0; 
   START_flag[nch] = 1; 
   STOP_flag[nch] = 0;
//делаем имитацию того, что уже получен старт, так как ответ будет без старта
 }
}   

unsigned char get_t_in(unsigned char nch)
{
 return(t_in[nch]);
}

void volkovsend(unsigned char nch, unsigned char *curbuf, unsigned long cursize)
{
	unsigned char chksum;
	unsigned long i, j, tmp;

  if (volkov_flag[nch]==0)
  {
	for (i = 0; i < cursize; i++)
	{
         com_putchar(nch, curbuf[i]);
	}
  }
  else
  {
// Clear transmit interrupt and buffer
        BeginSend(nch);
	chksum = AddressPLC[nch];
	put_char(nch, chksum);
	for (i = 0; i < cursize; i++)
	{
  	 chksum^=curbuf[i];
	 put_char(nch, curbuf[i]);
	}
     if (flag32[nch]==1)
     {
        if ((chksum==START)||(chksum==STOP)||(chksum==SHIFT)) tmp=28-get_t_in(nch); else tmp=29-get_t_in(nch);
        for(j=0;j<tmp;j++) put_char(nch, 0);
     }   
	put_char(nch, chksum);
        FinishSend(nch);
  }
}

long int volkovread(unsigned char nch, unsigned char *curbuf, unsigned long cursize)//распаковка принятого массива
{
	unsigned char chksum=0, m;
	unsigned long i, j, k;
  if (volkov_flag[nch]==0)
  {
	k=0;
	j=r_in[nch];
	r_in[nch]=0;
	for (i = 0; i < j; i++)
	{
         curbuf[k]=rbuf[nch][i];
 	 k++;
	 if (k>=cursize) return(-2);					// small output buffer
        }
  }
  else
  {
	if (STOP_flag[nch]==0) return(-1);				// read is not complite
	STOP_flag[nch]=0;
	j=r_in[nch];
	r_in[nch]=0;
	i=0;
	if (ma_sl_flag[nch]==0) if (rbuf[nch][i++]!=START) return(-3);			// START not found
	if (rbuf[nch][i]!=SHIFT) m=rbuf[nch][i]; else m=rbuf[nch][++i]+START;
	i++;
	if (m!=AddressPLC[nch]) return(-4);		// illegal Address
	chksum=m;
	k=0;
	do
	{
	 if (rbuf[nch][i]!=SHIFT) m=rbuf[nch][i]; else m=rbuf[nch][++i]+START;
	 i++;
	 chksum^=m;
	 curbuf[k]=m;
 	 if (k>cursize) return(-2);					// small output buffer
 	 k++;
	} while (i<(j-1));
	if (chksum==0) k--; else return(-5);					// illegal checksum
  }	
	return(k);
}

unsigned char com_read_ready(unsigned char nch)
{
 if (volkov_flag[nch]==0)
 {
  if (com_rbuflen(nch)>0) return 1; else return 0;
 }
 else
 {
  return STOP_flag[nch];
 }
}
unsigned char get_live_flag(unsigned char nch) {return STOP_echo_flag[nch];}
unsigned char com_tbuflen (unsigned char nch) {return (t_in[nch] - t_out[nch]);}
unsigned char com_rbuflen (unsigned char nch) {return (r_in[nch] - r_out[nch]);}

void com_putchar(unsigned char nch, unsigned char c)
{
  tbuf[nch][t_in[nch]++] = c;
  wait_end_of_send(nch);
}

unsigned char com_getchar(unsigned char nch)
{
 return(rbuf[nch][(r_out[nch]++) & (RBUF_SIZE - 1)]);
}

void Set_adr_PLC(unsigned char nch, unsigned char adr)
{
 AddressPLC[nch]=adr;
}

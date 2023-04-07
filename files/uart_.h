#ifndef UART_H
#define UART_H

void InitUart(void);
void SendByte(unsigned char nch, unsigned char value);
void RecvByte(unsigned char nch, unsigned char *value);


#endif
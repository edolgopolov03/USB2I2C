#ifndef spi_h
#define spi

void SPI_Init(void);
unsigned short SPI_MasterSend(unsigned char byte1, unsigned char byte2);
//void SPI_ISR (void);

#endif

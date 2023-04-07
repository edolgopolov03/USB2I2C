#ifndef uart_h
#define uart_h

#define BAUDRATE		9600            // baud rate of UART in bps
#define PLC_ADR			0
#define RI1				0
#define TI1				1
#define RBX1			2
#define TBX1			3
#define REN1			4
#define THRE1			5
#define PERR1			6
#define OVR1			7

sbit RE0     = P1^1;
sbit DE0     = P1^2;
sbit RE1     = P1^4;
sbit DE1     = P1^5; 

#define TYPE_COM		0	//1 - send by UART; 2 - change BAUDRATE(high = OUT_PACKAGE, low = OUT_PACKAGE+1); 3 - change TIMEOUT;
#define N_UART			1
#define NUM_SEND		2
#define NUM_RECV_WAIT	3		
#define OUT_PACKAGE		4
#define NUM_RECV		128
#define IN_PACKAGE		129

extern unsigned char i_flag;
extern unsigned char end_int;
extern unsigned char inside_interrupt;
extern unsigned char TIMEOUT_WAIT;
extern unsigned char TIMEOUT_BYTE;
//sbit ES1 = 0xE7;
sfr16 SBRL1     = 0xB4; 

void UART0_Init (int baud);
void UART1_Init (int baud);
void INT0_Init(void);
void UART1_ISR(void);
void UART0_ISR(void);
void INT0_ISR(void);
void Timer2_ISR(void);
void Finish_int(void);
void Timer_Init(void);
void Wait_Set(void);
void Byte_Set(void);

#endif

#include "ComEngine.h"
#include <stdio.h>
#include "uart.h"

xdata unsigned char buf [10];
xdata int i;

void SendByte_volkovStyle(unsigned char nch, unsigned char adr, int N)
{
	unsigned char xSum = (unsigned char) 0;
	SendByte(nch, START); // SendStart
	SendByte(nch, adr);  // Send Adress of Device
	xSum = adr;
	for (i = 0; i<N; i++)
	{
		if (buf[i]!=START & buf[i]!=STOP & buf[i]!=SHIFT)
		{
			SendByte(nch, buf[i]); //Send Package if it's not START STOP or SHIFT
		} else
		{
			SendByte(nch, SHIFT);
			SendByte(nch, buf[i] - START);
		}
		xSum = xSum^buf[i];
	}
	SendByte(nch, xSum);
	SendByte(nch, STOP);
}


void clear_buf()
{
	for (i = 0; i<10; i++)
	{
		buf[i] = 0;
	}
}

int identify()
{
	clear_buf();
	buf[0] = 1;
	return 1;	 
}

int repeat_last_ans()
{
	clear_buf();
	buf[0] = 2;
	return 1;	 
}

int get_state()
{
	clear_buf();
	buf[0] = 3;
	return 1;	 
}

int move(long Nstep, unsigned char STOPCOND)
{
	clear_buf();
	buf[0] = 4;
	buf[1] = Mask&(Nstep >> 24);
	buf[2] = Mask&(Nstep >> 16);
	buf[3] = Mask&(Nstep >> 8);
	buf[4] = Mask&(Nstep);
	buf[5] = STOPCOND;
	return 6;	 
}

int move_without_ac(long Nstep, unsigned char STOPCOND)
{
	clear_buf();
	buf[0] = 5;
	buf[1] = Mask&(Nstep >> 24);
	buf[2] = Mask&(Nstep >> 16);
	buf[3] = Mask&(Nstep >> 8);
	buf[4] = Mask&(Nstep);
	buf[5] = STOPCOND;
	return 6;
}

int config(unsigned char mov_cur, unsigned char hold_cur, unsigned char hold_delay, unsigned char CFG)
{
	clear_buf();
	buf[0] = 6;
	buf[1] = mov_cur; // 0-0A, 1-0.2A, 2-0.3A, 3-0.5A, 4-0.6A, 5-1.0A, 6-2.0A, 7-3.5A 
	buf[2] = hold_cur;// 0-0A, 1-0.2A, 2-0.3A, 3-0.5A, 4-0.6A, 5-1.0A, 6-2.0A, 7-3.5A 
	buf[3] = hold_delay;// time[s] = hold_delay / 30
	buf[4] = CFG; // [AccLeave, LeaveK, SoftK, sensor, K+, K-, 0, half]
	return 5;
} 

int speed(unsigned int min_speed, unsigned int max_speed, unsigned int acceleration)
{
	clear_buf();
	buf[0] = 7;
	buf[1] = Mask&(min_speed >> 8);
	buf[2] = Mask&(min_speed);
	buf[3] = Mask&(max_speed >> 8);
	buf[4] = Mask&(max_speed);
	buf[5] = Mask&(acceleration >> 8);
	buf[6] = Mask&(acceleration);
	return 7;
}

int forced_stop()
{
	clear_buf();
	buf[0] = 8;
	return 1;	 
}

int off_curr()
{
	clear_buf();
	buf[0] = 9;
	return 1;	 
}

int save_param()
{
	clear_buf();
	buf[0] = 10;
	return 1;	 
}

int set_imp(unsigned int N, unsigned int Nstart, unsigned int Nrepeat)
{
	clear_buf();
	buf[0] = 11;
	buf[1] = Mask&(N >> 8);
	buf[2] = Mask&(N);
	buf[3] = Mask&(Nstart >> 8);
	buf[4] = Mask&(Nstart);
	buf[5] = Mask&(Nrepeat >> 8);
	buf[6] = Mask&(Nrepeat);
	return 7;
}

int missed_steps()
{
	clear_buf();
	buf[0] = 12;
	return 1;	 
}

int read_config()
{
	clear_buf();
	buf[0] = 13;
	return 1;	 
}

int read_speed()
{
	clear_buf();
	buf[0] = 14;
	return 1;	 
}

int freq_test()
{
	clear_buf();
	buf[0] = 15;
	return 1;	 
}


int calibration(long Period)
{
	clear_buf();
	buf[0] = 16;
	buf[1] = Mask&(Period >> 24);
	buf[2] = Mask&(Period >> 16);
	buf[3] = Mask&(Period >> 8);
	buf[4] = Mask&(Period);
	return 5;	 
}

int move_precision_accuracy(long Nstep, long dt)
{
	clear_buf();
	buf[0] = 17;
	buf[1] = Mask&(Nstep >> 24);
	buf[2] = Mask&(Nstep >> 16);
	buf[3] = Mask&(Nstep >> 8);
	buf[4] = Mask&(Nstep);
	buf[5] = Mask&(dt >> 24); // dt - time between steps in nanoseconds
	buf[6] = Mask&(dt >> 16);
	buf[7] = Mask&(dt >> 8);
	buf[8] = Mask&(dt);
	return 9;	 
}

int output_imp()
{
	clear_buf();
	buf[0] = 18;
	return 1;	 
}

int set_abs_coord(long coord)
{
	clear_buf();
	buf[0] = 19;
	buf[1] = Mask&(coord >> 24);
	buf[2] = Mask&(coord >> 16);
	buf[3] = Mask&(coord >> 8);
	buf[4] = Mask&(coord);
	return 5;	 
}

int get_abs_coord()
{
	clear_buf();
	buf[0] = 20;
	return 1;	 
}

int syncr(unsigned char flags)
{
	clear_buf();
	buf[0] = 21;
	buf[1] = flags; // [0,0,0,0,0,go, block, broadcast]
	return 2;	 
}

int set_addr_2(unsigned int num, unsigned char rate, unsigned char addr)
{
	// BAUDRATE = 600
	clear_buf();
	buf[0] = 0;
	buf[1] = START;
	buf[2] = 'W';
	buf[3] = 'S';
	buf[4] = Mask&(num >> 8);
	buf[5] = Mask&(num);
	buf[6] = rate; // 0-1200;1-2400;2-4800;3-9600;4-19200;5-38400;6-57600
	buf[7] = addr;
	return 8;
}

int set_addr_1(unsigned char rate, unsigned char addr)
{
	// BAUDRATE = 600
	clear_buf();
	buf[0] = 0x55;
	buf[1] = rate; // 0-1200;1-2400;2-4800;3-9600;4-19200;5-38400;6-57600
	buf[2] = addr;
	return 3;
}

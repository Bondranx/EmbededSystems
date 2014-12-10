#include <hidef.h>
#include "derivative.h"
#include "ATD_Lib.h"

void ATD0_Init(void)
{
	ATD0DIEN &= 0b11111110;	//ensure A to D setting for AN0
	DDR1AD0 &= 0b11111110;	//AN0 as an input
	ATD0CTL2 = 0b11100000;	//Power up, fast flag, halt in wait, no trig, no interrupts
	asm LDX #134;			//50 us wait for power-up
	asm DBNE X,*;
	ATD0CTL3 = 0b00001010;	//single conversion, no FIFO, complete on wait
	ATD0CTL4 = 0b00100110;	//10 bit, 4 clocks per sample, PRS = 6
}

int ATD_AN0(void)
{
	ATD0CTL5 = 0b10000000;	//right justified, unsigned, single conv, one channel, result in ATD0
	while ((ATD0STAT0&0b10000000)==0);
	return ATD0DR0;
}

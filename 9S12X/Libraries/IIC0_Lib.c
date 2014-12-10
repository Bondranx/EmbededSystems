//IIC0 Bus Library
//Processor: MC9X12SCP512
//Crystal:   16MHz
//by:        Brandon Foote
//date:      November 25, 2014

#include  <hidef.h>
#include  "derivative.h"
#include  "IIC0_Lib.h"

void IIC0_Init(void) 
{
     IIC0_IBFD=0x47;  //100kHz, SDA hold = 20 cks, SCL Hold start = 32 SCL, Hold Stop = 42
     IIC0_IBCR|=0b10000000; //Enable the bus - must be done first 
     IIC0_IBCR&=0b10111110; //no interrupts, normal WAI
}
void IIC0_Write(byte bAddr, byte bReg, byte bData) 
{
     while(IIC0_IBSR & 0b00100000);
     IIC0_IBCR |= 0b00110000;
     
     IIC0_IBDR = bAddr & 0b11111110;
     while(!(IIC0_IBSR & 0b00000010));
     IIC0_IBSR |= 0b00000010;
     
     IIC0_IBDR = bReg;
     while(!(IIC0_IBSR & 0b00000010));
     IIC0_IBSR |= 0b00000010;
     
     IIC0_IBDR = bData;
     while(!(IIC0_IBSR & 0b00000010));
     IIC0_IBSR |= 0b00000010;
     
     IIC0_IBCR &= 0b11001111;
}
//byte IIC0_Read(byte bAddr, byte bReg) 
//{
    
//}

void WriteDAC(byte bAddr, byte bCommand, int iData)
{
     iData*=16;     //Move data into upper 12 bits
     
     while(IIC0_IBSR & 0b00100000); //wait for busy flag
     IIC0_IBCR |= 0b00110000;       //micro as master, start transmitting
     
     IIC0_IBDR = bAddr & 0b11111110;    //place address on bus with /write
     while(!(IIC0_IBSR & 0b00000010));  //wait for flag
     IIC0_IBSR |= 0b00000010;           //clear flag
     
     IIC0_IBDR = bCommand;                  //send the desired command
     while(!(IIC0_IBSR & 0b00000010));  //wait for flag
     IIC0_IBSR |= 0b00000010;           //clear flag
     
     IIC0_IBDR = (byte)(iData/256);     //send first byte of data
     while(!(IIC0_IBSR & 0b00000010));   //wait for flag
     IIC0_IBSR |= 0b00000010;           //clear flag
     
     IIC0_IBDR = (byte)(iData&0b0000000011111111); //send second byte of data
     while(!(IIC0_IBSR & 0b00000010));               //wait for flag
     IIC0_IBSR |= 0b00000010;                        //clear flag
     
     IIC0_IBCR &= 0b11001111;    //Stop transmitting, exit master mode
}
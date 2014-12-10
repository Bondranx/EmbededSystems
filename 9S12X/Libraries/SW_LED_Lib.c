//Switches and LEDs
//Processor:  MC9S12SCP512
//Crystal:    16MHz
//by:         Brandon Foote
//date:       November 25,2014

#include  <hidef.h>
#include  "derivative.h"
#include  "SW_Led_Lib.h"

void  SW_LED_Init(void) 
{ 
  DDR1AD1   =   0b11100000;        //LEDs as output, switches as inputs
  PT1AD1    &=  0b00011111;        //Start with LEDs off
  ATD1DIEN1 |=  0b00011111;        //Connect the switches
}

byte  SwCk(void) 
{
  byte  bSample1=1;
  byte  bSample2=0;
  
  while(bSample1!=bSample2) 
  {
    bSample1=PT1AD1&0b00011111;
    asm   LDX #26667;
    asm   DBNE X,*;
    bSample2=PT1AD1&0b00011111;  
  }
  return  bSample1;
}
/********************************************************************/
// HC12 Program:	YourProg - MiniExplanation
// Processor:		MC9S12XDP512
// Xtal Speed:		16 MHz
// Author:			This B. You
// Date:			LatestRevisionDate

// Details: A more detailed explanation of the program is entered here
/********************************************************************/


#include <hidef.h>      		/* common defines and macros 		*/
#include "derivative.h"      	/* derivative-specific definitions 	*/
/********************************************************************/
//		Library includes
/********************************************************************/

//#include "Your_Lib.h"
  #include "SevSeg_Lib.h"
  #include "ATD_Lib.h"
/********************************************************************/
//		Prototypes
/********************************************************************/
int Hex2BCD(int iHex);
void TimerSetup(int iTimeVal, byte bPrescaler);
void TimerInterval(int iTimeVal);


/********************************************************************/
//		Variables
/********************************************************************/


/********************************************************************/
//		Lookups
/********************************************************************/


void main(void)
{
// main entry point
_DISABLE_COP();

/********************************************************************/
// initializations
/********************************************************************/
  ATD0_Init();
  SevSeg_Init();
	for (;;)
	{
	  SevSeg_Top4(ATD_AN0());
	  SevSeg_Low4(Hex2BCD(ATD_AN0()*5));
	}

}



/********************************************************************/
//		Functions
/********************************************************************/
 
 
 int Hex2BCD(int iHex) 
 {
    byte  bMSB;
    byte  b2SB;
    byte  b3SB;
    byte  bLSB;
    
    bMSB = (byte)(iHex/1000);
    iHex = iHex-1000*bMSB;
    b2SB = (byte)(iHex/100);
    iHex = iHex-100*b2SB;
    b3SB = (byte)(iHex/10);
    bLSB = iHex-10*b3SB;
    
    return (bMSB*4096+b2SB*256+b3SB*16+bLSB);
 }

  void  TimerSetup(int iTimeVal, byte bPrescaler)
{
  
  TSCR1     |=  0b10000000;        //turn timer on
  TSCR2     &=  0b11111000;        //start with prescaler cleared
  TSCR2     |=  bPrescaler;        //put the prescaler value in place
  TIOS      |=  0b00000001;        //Output compare on channel 0
  TCTL2     &=  0b11111100;        //Start with output mode for ch 0 clear
  TCTL1     |=  0b00000001;        //"01" is toggle mode fo ch 0
  TC0       =   TCNT+iTimeVal;     //set the first interval
  TFLG1     |=  0b00000001;        //clear the flag for ch 0
  TIE       |=  0b00000001;        //Ch0 timer enable
  while(!(TFLG1 && 0b00000001));   //Wait for flag from timer channel 0
}

void  TimerInterval(int iTimeVal) 
{
  TC0     +=  iTimeVal;
	TFLG1   |=  0b00000001;        //clear the flag
	while(!(TFLG1 && 0b00000001)); //Wait for flag from timer channel 0
}


/********************************************************************/
//		Interrupt Service Routines
/********************************************************************/


/********************************************************************/
// HC12 Program:	ICA 13 
// Processor:	  	MC9S12XDP512
// Xtal Speed:		16 MHz
// Author:		  	Brandon Foote
// Date:		    	November 25,2014
// Details:       LED binary up counter
/********************************************************************/


#include <hidef.h>      		  /* common defines and macros*/
#include "derivative.h"      	/* derivative-specific definitions*/
/********************************************************************/
//		Library includes
/********************************************************************/

//#include "Your_Lib.h"
  #include "SW_LED_Lib.h"

/********************************************************************/
//		Prototypes
/********************************************************************/
void TimerSetup(int iTimeVal, byte bPrescaler);
//void TimerInterval(int iTimeVal);

/********************************************************************/
//		Variables
/********************************************************************/
   
volatile int  iTimeVal=62500;
byte bPrescaler=5;


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
  SW_LED_Init();
  TimerSetup(iTimeVal,bPrescaler);
  EnableInterrupts;

	for (;;)
	{
	//main program loop
	  if(SwCk()==0b00000001) PT1AD1&=0b00011111;
	  else PT1AD1  +=  0b00100000;
	  asm WAI;
	  //TimerInterval(iTimeVal);
	}

}



/********************************************************************/
//		Functions
/********************************************************************/
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

//void  TimerInterval(int iTimeVal) 
//{
  //TC0     +=  iTimeVal;
//	TFLG1   |=  0b00000001;        //clear the flag
//	while(!(TFLG1 && 0b00000001)); //Wait for flag from timer channel 0
//}


/********************************************************************/
//		Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vtimch0 void TimerInterval(void) 
{ 
 TC0 =(int)(iTimeVal+TC0); // next time 
 TFLG1 |= 0b00000001; // acknowledge interrupt 
} 

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
  #include "SW_LED_Lib.h"
  #include "ATD_Lib.h"


/********************************************************************/
//		Prototypes
/********************************************************************/
void TimerSetup(int iTimeVal, byte bPrescaler);
int Hex2BCD(int iHex);
void OneSecHandler(void);
void ATDHandler(void);
void AnnoyingNoise(void);

/********************************************************************/
//		Variables
/********************************************************************/
volatile int  iTimeVal=2500;
byte bPrescaler=5;
int TimerCount=0;
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
  SevSeg_Init();
  ATD0_Init();
  
  PWMCLK &=0b10111111;
  PWMCLK|= 0b01000000;
  PWMPOL|=0b01000000;
  PWMPRCLK&=0b10001111;
  PWMPRCLK|=0b00010000;
  PWMPER6=100;
  PWMSCLB=38;
  
  PWME|=0b01000000;
  
  
  TimerSetup(iTimeVal,bPrescaler);
  
  EnableInterrupts;
	for (;;)
	{
	  ATDHandler();
    OneSecHandler();
    AnnoyingNoise();
	  asm WAI;     
	
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

void OneSecHandler(){ 
  TimerCount+=1;
    if(TimerCount%100==0) 
    { 
        SevSeg_Top4(Hex2BCD(TimerCount/100));
    }    
}

void ATDHandler(){
  SevSeg_Low4(Hex2BCD(ATD_AN0()*5)); 
}

void AnnoyingNoise(){
    PWMPER6 = 100-(ATD_AN0()*50)/1023;
    PWMDTY6 = PWMPER6/2;    
}

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


/********************************************************************/
//		Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vtimch0 void TimerInterval(void) 
{ 
 TC0 =(int)(iTimeVal+TC0); // next time 
 TFLG1 |= 0b00000001; // acknowledge interrupt 
} 

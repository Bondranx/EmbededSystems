;************************************************************************
;* HC12 Program:	YourProg - MiniExplanation							*
;* Processor:	MC9S12XDP512											*
;* Xtal Speed:	16 MHz													*
;* Author:		P Ross Taylor											*
;* Date:		LatestRevisionDate										*
;*																		*
;* Details: A more detailed explanation of the program is entered here	*
;*																		*
;************************************************************************

;export symbols
		XDEF 		Entry			;export'Entry' symbol
		ABSENTRY 	Entry			;for absolute assembly: app entry point

;include derivative specific macros
		INCLUDE 'derivative.inc'

;********************************************************************
;*		Equates														*
;********************************************************************


;********************************************************************
;*		Variables													*
;********************************************************************
		ORG			RAMStart		;Address $2000


;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:
         BCLR   PWMCLK,%01000000
         BCLR   PWMPRCLK,%01110000
         MOVB   #128,PWMPER6
         MOVB   #64,PWMDTY6
         BSET   PWME,%01000000
         
         CLRB               ;Divide by 1 prescale timer
         LDX    #200       ;200*125ns=25us interval per sample
         JSR    IntXInitB   ;First Interval
         BRA    Sine
Ramp:    LDAA   #1          ;bottom limit
UpRamp:  STAA   PWMDTY6     ;set duty cycle
         JSR    IntX        ;Next Interval
         INCA
         BMI    Ramp        ;reached 128? Restart Ramp
         BRA    UpRamp      ;othewise keep ramping
Sine:
         LDY    #SineWave
SineStart:
         CLRA               ;Pointer to table
Sin:     LDAB   A,Y         ;get value from table
         STAB   PWMDTY6     ;set duty cycle
         JSR    IntX        ;next Interval
         INCA   
         CMPA   #180
         BEQ    SineStart    ;end of 128-part lookup table
         BRA    Sin

         ;LDAB   #7
         ;LDX    #62500
         ;JSR    IntXInitB
         ;BSET   PWMCLK,%01000000
         ;BSET   PWMPOL,%000010011
;         BSET   PWMSCLA,%100
;         BCLR   PWMPRCLK,%00000010
;         BSET   PWMPRCLK,%00000010
;         MOVB   #50,PWMSCLB
;         MOVB   #100,PWMPER0
;         MOVB   #100,PWMPER1
;         MOVB   #100,PWMPER4
;         MOVB   #10,PWMDTY0
;         MOVB   #10,PWMDTY1
;         MOVB   #10,PWMDTY4 
;Loop:    BSET   PWME,#%00010000
;         JSR    IntX
;         BCLR   PWME,#%00010000
;         BSET   PWME,#%00000010
;         JSR    IntX
;         BCLR   PWME,#%00000010
;         BSET   PWME,#%00000001
;         JSR    IntX
;         BCLR   PWME,#%00000001
;         BSET   PWME,#%00010011
;         JSR    IntX
;         BCLR   PWME,#%00010011 
;         BRA    Loop     
          




;********************************************************************
;*		Subroutines													*
;********************************************************************


;********************************************************************
;*		Interrupt Service Routines											*
;********************************************************************


;********************************************************************
;*		Constants													*
;********************************************************************
		ORG			ROM_C000Start	;second block of ROM


;********************************************************************
;*		Look-Up Tables												*
;********************************************************************
  SineWave:		DC.B	64,66,68,70,72,74,77,79,81,83,85,87,89,91,93,95
		          DC.B	97,99,101,102,104,106,107,109,110,112,113,114,116,117,118,119
	      	    DC.B	120,121,122,123,123,124,125,125,126,126,126,126,126,127,126,126
	      	    DC.B	126,126,126,125,125,124,123,123,122,121,120,119,118,117,116,114
	      	    DC.B	113,112,110,109,107,106,104,102,101,99,97,95,93,91,89,87
	      	    DC.B	85,83,81,79,77,74,72,70,68,66,64,61,59,57,55,53
	      	    DC.B	50,48,46,44,42,40,38,36,34,32,30,28,26,25,23,21
	      	    DC.B	20,18,17,15,14,13,11,10,9,8,7,6,5,4,4,3
	      	    DC.B	2,2,1,1,1,1,1,1,1,1,1,1,1,2,2,3
	      	    DC.B	4,4,5,6,7,8,9,10,11,13,14,15,17,18,20,21
	      	    DC.B	23,25,26,28,30,32,34,36,38,40,42,44,46,48,50,53
	      	    DC.B	55,57,59,61


;********************************************************************
;*		SCI VT100 Strings											*
;********************************************************************


;********************************************************************
;*		Absolute Library Includes									*
;********************************************************************

		;INCLUDE "Your_Lib.inc"
		INCLUDE   Misc_Lib.inc


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector

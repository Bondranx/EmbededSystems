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
    TimCounter:   ds.w  1

;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:
       JSR    SCI0Init192
       BCLR   DDRJ,%00000001  ;Makes DDRJ and input
       BSET   PPSJ,%00000001  ;Sets rising edge response
       BSET   SCI0CR2,%00100000
       BSET   SCI0SR1,%00100000
       BSET   TIE,%00000001   ;Enable OC0 interrupt
       BSET   TIE,%00100000   ;Enable SCI0 interrupt
       BSET   PIEJ,%00000001  ;Enable Port J interrupt
       BSET   PIFJ,%00000001  ;Clears Port J interrupt flag
       CLI                    ;Enable Interrupts
      
       JSR    SevSeg_Init
       
       MOVB   #%10000000,TSCR1;Enable Timer Module
       LDAB   #%00000111      ;2^7 presacler
       STAB   TSCR2           ;Prescaler set to Bus/(2^B)
       MOVB   #%00000001,TIOS ;set IOS0 for output compare
       MOVB   #%00000001,TCTL2;toggle mode for PT0
       LDD    #6250           ;100ms
       ADDD   TCNT            ;set new event timer value based on clock
       STD    TC0
       BSET   TFLG1,%00000001 ;clear flag
       LDD    #0
RegularLoop:
       LDX    #0
       DBNE   X,*
       ADDD   #1
       JSR    SevSeg_Low4
       BRA    RegularLoop
         
          




;********************************************************************
;*		Subroutines													*
;********************************************************************


;********************************************************************
;*		Interrupt Service Routines											*
;********************************************************************

;********************************************************************
;*          Timer_ISR
;********************************************************************

Timer_ISR:
            BSET  TFLG1,%00000001
            LDD   #6250
            ADDD  TC0
            STD   TC0
            LDD   TimCounter
            ADDD  #1
            STD   TimCounter
            JSR   SevSeg_Top4
            RTI
SCI_Echo_ISR:
            BSET  SCI0SR1,%00100000 ;Clear Interrupt Flag
            LDAA  SCI0DRL 
            JSR   SCI0_TxByte
            RTI   
Switch_ISR:
            BSET  PIFJ,%00000001    ;Clear Interrupt Flag
            MOVW  #0,TimCounter
            RTI      

;********************************************************************
;*		Constants													*
;********************************************************************
		ORG			ROM_C000Start	;second block of ROM


;********************************************************************
;*		Look-Up Tables												*
;********************************************************************



;********************************************************************
;*		SCI VT100 Strings											*
;********************************************************************


;********************************************************************
;*		Absolute Library Includes									*
;********************************************************************

		;INCLUDE "Your_Lib.inc"
		INCLUDE   Misc_Lib.inc
		INCLUDE   SevSeg_Lib.inc
		INCLUDE   SCI0_Lib.inc


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG     $FFCE       ;Switch Interrupt
		DC.W    Switch_ISR
		ORG     $FFD6       ;SCI0 Interrupt
		DC.W    SCI_Echo_ISR
		ORG     $FFEE       ;Tim0 Interrupt
		DC.W    Timer_ISR
		ORG			$FFFE
		DC.W		Entry			;Reset Vector
		

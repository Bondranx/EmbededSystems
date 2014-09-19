;************************************************************************
;* HC12 Program:	YourProg - MiniExplanation						              	*
;* Processor:	MC9S12XDP512											                        *
;* Xtal Speed:	16 MHz												                        	*
;* Author:		Brandon Foote										                          *
;* Date:		LatestRevisionDate										                      *
;*																		                                  *
;* Details: A more detailed explanation of the program is entered here	*
;*																		                                  *
;************************************************************************

;export symbols
		XDEF 		Entry			;export'Entry' symbol
		ABSENTRY 	Entry			;for absolute assembly: app entry point

;include derivative specific macros
		INCLUDE 'derivative.inc'

;********************************************************************
;*		Equates														                            *
;********************************************************************


;********************************************************************
;*		Variables													                            *
;********************************************************************
		ORG			RAMStart		;Address $2000


;********************************************************************
;*		Code Section												                          *
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:       JSR SevSeg_Init

Loop:       LDAA PT1AD1
            BCLR PT1AD1,%10000000 
            BCLR PT1AD1,%00100000
            BCLR PT1AD1,%01000000
            BRSET PT1AD1,%00010000,UP            
LFT:        BRSET PT1AD1,%00001000,LEFT
RGHT:       BRSET PT1AD1,%00000010,RIGHT
MIDD:       BRSET PT1AD1,%00000001,MID
            BRA Loop
            
UP:         
                                   
LEFT:       BSET PT1AD1,%10000000
                       
            BRA RGHT
RIGHT:      BSET PT1AD1,%00100000
            
            BRA MIDD
MID:        BSET PT1AD1,%01000000
            
            BRA Loop
DOWN:       BSET PT1AD1,%11100000     
            BRA Loop
            
           
           ;LDX #SevSeg8Const
           LDAA #%10101100
           LDAB #$00
           JSR SevSeg_Cust
           BRA *
           
           
;********************************************************************
;*		Subroutines												                          	*
;********************************************************************


;********************************************************************
;*		Interrupt Service Routines											              *
;********************************************************************


;********************************************************************
;*		Constants													                            *
;********************************************************************
		ORG			ROM_C000Start	;second block of ROM
		SevSeg8Const: dc.b  $D,$E,$A,$D,$B,$E,$E,$F


;********************************************************************
;*		Look-Up Tables												                        *
;********************************************************************


;********************************************************************
;*		SCI VT100 Strings											                        *
;********************************************************************


;********************************************************************
;*		Absolute Library Includes									                    *
;********************************************************************

		INCLUDE "SevSeg_Lib.inc"


;********************************************************************
;*		Interrupt Vectors											                        *
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector

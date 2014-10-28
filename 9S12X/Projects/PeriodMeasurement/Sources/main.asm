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
		TempCond: ds.w 1
		TempCondBCD: ds.w 1
    FirstTwoASCII: ds.w 1
    SecondTwoASCII: ds.w 1
;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:
       JSR    SevSeg_Init
       JSR    LCD_Init
       JSR    SetPT7
       JSR    TimInit1us
FlagLoop:       
       BRCLR  TFLG1,%10000000,*
       BSET   TFLG1,#%10000000
       LDY    TC7
       TFR    Y,D
       SUBD   TempCond
       STY    TempCond
       JSR    SevSeg_Top4
       PSHD
       LDAA   #1
       LDX    #FirstLine
       JSR    LCD_String
       LDD    #TempCond
       JSR    Hex2BCD
       STD    TempCondBCD
       JSR    Hex2Asc2
       STD    FirstTwoASCII
       LDD    TempCondBCD
       EXG    B,A
       JSR    Hex2Asc2
       STD    SecondTwoASCII
       LDAA   #64
       LDX    #FirstTwoASCII
       JSR    LCD_String
       LDX    #SecondTwoASCII
       JSR    LCD_String
       PULD
       BRA    FlagLoop


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
		FirstLine:    dc.b "Period=:",0
		
;*******************************************************************
;*		Look-Up Tables												*
;********************************************************************


;********************************************************************
;*		SCI VT100 Strings											*
;********************************************************************


;********************************************************************
;*		Absolute Library Includes									*
;********************************************************************

		;INCLUDE "Your_Lib.inc"
		INCLUDE LCD_Lib.inc
	  INCLUDE Misc_Lib.inc
	  INCLUDE SW_LED_Lib.inc
	  INCLUDE SevSeg_Lib.inc


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector
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
    Null: ds.b 1
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
       MOVB   #0,Null
       JSR    LCD_CursOff
FlagLoop:       
       BRCLR  TFLG1,%10000000,*
       BSET   TFLG1,#%10000000
       LDY    TC7
       TFR    Y,D
       SUBD   TempCond
       STY    TempCond
       JSR    SevSeg_Top4
       PSHA
       LDAA   #0
       JSR    LCD_Addr
       PULA
       LDX    #FirstLine
       JSR    LCD_String
       
       JSR    Hex2BCD
       JSR    SevSeg_Low4
       STD    TempCondBCD
       JSR    Hex2Asc2
       STD    FirstTwoASCII
       LDD    TempCondBCD
       EXG    B,A
       JSR    Hex2Asc2
       STD    SecondTwoASCII
       PSHA
       LDAA   #64
       JSR    LCD_Addr
       PULA
       LDX    #FirstTwoASCII
       JSR    LCD_String
       PSHA
       LDAA   #69
       JSR    LCD_Addr
       PULA
       LDX    #ThirdLine
       JSR    LCD_String
       
       

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
		FirstLine:    dc.b "Period=",0
		ThirdLine:    dc.b "us",0
		
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
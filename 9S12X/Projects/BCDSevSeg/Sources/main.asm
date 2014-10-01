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
		Var1: ds.b 1


;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:
        LDAA #$56
        LDAB #$0
        STAA Var1
         
        
        JSR SevSeg_Init
        JSR SevSeg_BlAll
Loop:   LDAA Var1
        INCA
        CMPA #99
        BLS DontRoll
        CLRA
DontRoll:  STAA Var1
 
        LDAB #5
        MUL
        JSR Hex2BCD
        JSR SevSeg_Low4
        LDAB Var1
        CLRA
        JSR Hex2BCD
        EXG B,A
        LDAB #2
        JSR SevSeg_Two
        JSR Timer2s
        BRA Loop


Timer2s:
           LDAB   #89
Timloop:   LDY    #9925
           DBNE   Y,*
           DBNE   B,Timloop
           RTS              

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


;********************************************************************
;*		SCI VT100 Strings											*
;********************************************************************


;********************************************************************
;*		Absolute Library Includes									*
;********************************************************************

		;INCLUDE "Your_Lib.inc"
		INCLUDE 'Misc_Lib.inc'
		INCLUDE 'SevSeg_Lib.inc'


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector

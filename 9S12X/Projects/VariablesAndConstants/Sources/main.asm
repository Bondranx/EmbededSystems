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
		Variable1: ds.b 2 
    Variable2: ds.b 1 
    FailConst: dc.b "This is going to disappear" 


;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:

    CLR Variable1 
    CLR Variable1+1
    CLR Variable2 
    LDX #GoodStr 
    LDAA #GoodStr 
    LDAA GoodStr 
    LDY GoodStr 
    LDAA 0,X 
    LDAB 5,X 
    LDD 5,X 
    STD Variable1 
    STAB Variable1 
    STAA Variable1+1 
    STAB Variable2 
    STD Variable2 
    STAA FailVar 
    INX 
    LDAA 0,X 
    BRA * 





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
		GoodStr: dc.b "This will not disappear" 
    AltStr: dc.b $48,'i',$64,"den",$20,"Me",$73,115,$60+1,'g',$67-2 
    FailVar: ds.b 1 


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


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector

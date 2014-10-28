;************************************************************************
;* HC12 Program:	YourProg - MiniExplanation							              *
;* Processor:	MC9S12XDP512											                        *
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
            JSR SCI0Init192
            LDX #FirstLine
            JSR SCI0_TxStr
            LDX #SecondLine
            JSR SCI0_TxStr
Char1:
            JSR SCI0_RxByte
            BCC Char1
            CMPA  #'0'
            BLO Char1
            CMPA  #'9'
            BHI Char1
            JSR SCI0_TxByte
            TFR A,B
            JSR Timer2s
Char2:
            JSR SCI0_RxByte
            BCC Char2
            CMPA  #'0'
            BLO Char2
            CMPA  #'9'
            BHI Char2
            JSR SCI0_TxByte
            JSR AsciiToHex
            EXG A,B
            JSR AsciiToHex
            LSLA
            LSLA
            ABA
            LDAB #50
            MUL
            JSR Hex2Asc2
            JSR SCI0_TxByte
            LDX #ThirdLine
            JSR SCI0_TxStr
            BRA *

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

FirstLine:    dc.b $1B,"[2;30HMultiply by 50",0
SecondLine:   dc.b $1B,"[4;20HEnter two digits: ",0
ThirdLine:    dc.b $1B,"[5;20HMultiplied by 50 = ",0
FourthLine:   dc.b $1B,"[7;30HPress enter to continue",0



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
		Include Misc_Lib.inc
		Include SCI0_Lib.inc


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector

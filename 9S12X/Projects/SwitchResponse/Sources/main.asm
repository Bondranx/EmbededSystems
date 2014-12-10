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
    TempState: ds.b 1

;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:
           LDAA   #0
           STAA   Var1
           JSR    SW_LED_Init
           JSR    SevSeg_Init
SwitchCk:
           JSR    Timer2s
           JSR    GrnLEDon
           JSR    ChkLeftSw
           BCC    RedOff
           JSR    RedLEDon
           BRA    RedOn
RedOff:    JSR    RedLEDoff
RedOn:     JSR    Timer2s
           JSR    GrnLEDoff
           JSR    SwCk
           CMPA   TempState
           BEQ    SwitchCk
           STAA   TempState
           BRSET  TempState,#%00000001,Mid
           BRSET  TempState,#%00000010,Right
           BRCLR  TempState,#%00000001,MidSkip
           BRCLR  TempState,#%00000010,RightSkip
           BRA    SwitchCk
Mid:       BRSET  PT1AD1,#%01000000,YelOff
           JSR    YelLEDon
           BRA    MidSkip
Right:     PSHA
           PSHB
           LDAA   Var1
           LDAB   #4
           INCA
           STAA   Var1
           CMPA   #9
           BGT    Reset
           JSR    SevSeg_Char
           PULB
           PULA
           BRA    RightSkip
Reset:     LDAA   #0
           STAA   Var1
           JSR    SevSeg_Char     
           BRA    RightSkip
     
YelOff:    JSR    YelLEDoff
RightSkip: 
MidSkip:   BRA    SwitchCk


Timer2s:
           LDAB   #89
Timloop:   LDY    #1000
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
		INCLUDE 'SW_LED_Lib.inc'


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector

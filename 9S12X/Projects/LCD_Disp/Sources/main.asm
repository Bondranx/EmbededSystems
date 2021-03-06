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
		SwCond: ds.b 1


;********************************************************************
;*		Code Section												*
;********************************************************************
		ORG			ROM_4000Start	;Address $4000 (FLASH)
Entry:
		LDS			#RAMEnd+1		;initialize the stack pointer

Main:
       JSR  SW_LED_Init
       JSR  LCD_Init
       JSR LCD_CursOff
SwitchCK:
       JSR  SwCk
       CMPA SwCond
       BEQ SwitchCK
       STAA SwCond
       BRSET  SwCond,#%00010000,Up
       BRSET  SwCond,#%00000100,Lower
       BRSET  SwCond,#%00000001,Mid
       BRCLR  SwCond,%00000001,SwitchCK
       BRCLR  SwCond,%00000100,SwitchCK
       BRCLR  SwCond,%00010000,SwitchCK
       BRA    SwitchCK
Up:
       JSR  LCD_Init
       JSR  LCD_CursOff
       LDX  #Flower
       JSR  LCD_CharGen8
       LDX  #FirstLine
       JSR  LCD_String
       LDAA #64
       JSR  LCD_Addr
       LDX  #SecondLine
       JSR  LCD_String
       LDAA  #$F
       EXG A,B
SwChkDnLoop:
       LDAA #30
       JSR  LCD_Addr
       EXG  B,A
       PSHA
       JSR  Hex2Asc1
       JSR  LCD_Char
       BRA SwitchCK
Lower: 
       PULA       
       EXG  A,B
       DECB
       CMPB #$FF
       BEQ  HitZero
       BRA  SwChkDnLoop
Mid:    
       LDAA  #0
CharLoop:
       PSHA
       LDAA  #95
       JSR   LCD_Addr
       PULA
       JSR   LCD_Char
       INCA
       JSR   Timer2s
       CMPA  #7
       BLS   CharLoop
       BRA  SwitchCK
HitZero:
       LDAB #$F 
       BRA  SwChkDnLoop


Timer2s:
           LDAB   #89
Timloop:   LDY    #5925
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
		FirstLine:    dc.b "Interactive LCD",0
		SecondLine:   dc.b "-By Brandon Foote",0
		
		Flower: dc.b  %00010001
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00010001
		        
		        dc.b  %00010001
		        dc.b  %00001010
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00000000
		        dc.b  %00001010
		        dc.b  %00010001
		        
		        dc.b  %00010001
		        dc.b  %00001010
		        dc.b  %00001010
		        dc.b  %00000100
		        dc.b  %00000100
		        dc.b  %00001010
		        dc.b  %00001010
		        dc.b  %00010001
		        
		        dc.b  %00010101
		        dc.b  %00001010
		        dc.b  %00001010
		        dc.b  %00010101
		        dc.b  %00010101
		        dc.b  %00001010
		        dc.b  %00001010
		        dc.b  %00010101
            
		        dc.b  %00010101
		        dc.b  %00001010
		        dc.b  %00001010
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00001010
		        dc.b  %00001010
		        dc.b  %00010101
            
		        dc.b  %00011111
		        dc.b  %00001110
		        dc.b  %00001110
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00001110
		        dc.b  %00001110
		        dc.b  %00011111
            
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00001110
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00001110
		        dc.b  %00011111
		        dc.b  %00011111
            
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00011111
		        dc.b  %00011111
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


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector

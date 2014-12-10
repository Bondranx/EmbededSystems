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
        JSR     LCD_Init
        JSR     SW_LED_Init
        LDAB    #7
        LDX     #1250
        JSR     IntXInitB
       
        BSET    PWME,%00001000
        BSET    PWMCLK,%00011011   ;Activate CLKs for LED and Backlight
        BSET    PWMPOL,%11111111   ;Set positive polarity
        BCLR    PWMPRCLK,%00011011
        BSET    PWMPRCLK,%00011011
        BSET    PWMSCLA,#50
        BSET    PWMSCLB,#100
        MOVB    #200,PWMPER0
        MOVB    #200,PWMPER1
        MOVB    #100,PWMPER3
        MOVB    #200,PWMPER4
        MOVB    #1,PWMDTY0
        MOVB    #1,PWMDTY1
        MOVB    #1,PWMDTY4
        MOVB    #50,PWMDTY3
FullLoop:
        BRSET   PT1AD1,#%00010000,Upper
        BRSET   PT1AD1,#%00000100,Lower
        BRA     BackLight
Upper:
        
        LDAA    PWMDTY3
        CMPA    #100
        BEQ     BackLight 
        ADDA    #1   
        STAA    PWMDTY3
        BRA     BackLight        
Lower:
        
        LDAA    PWMDTY3
        CMPA    #1
        BEQ     BackLight
        SUBA    #1
        STAA    PWMDTY3
        BRA     BackLight
BackLight:
        CMPB    #1
        BEQ     Red
        CMPB    #2
        BEQ     DimRed
Red:    
        JSR     IntX 
        LDAB    #1
        LDAA    PWMDTY4
        BCLR    PWME,#%00001010
        BSET    PWME,#%00011000
        CMPA    #10
        BEQ     DimRed
        ADDA    #1
        STAA    PWMDTY4
        BRA     FullLoop
DimRed:
        JSR     IntX
        LDAB    #2
        SUBA    #1
        STAA    PWMDTY4
        CMPA    #1
        BEQ     FullLoop
        
        BRA     FullLoop



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
 OnOff:   dc.b  %00011000,%00001010,%000001001,%00011011

;********************************************************************
;*		SCI VT100 Strings											*
;********************************************************************


;********************************************************************
;*		Absolute Library Includes									*
;********************************************************************

		;INCLUDE "Your_Lib.inc"
		INCLUDE   SW_LED_Lib.inc
		INCLUDE   Misc_Lib.inc
		INCLUDE   LCD_Lib.inc


;********************************************************************
;*		Interrupt Vectors											*
;********************************************************************

		ORG			$FFFE
		DC.W		Entry			;Reset Vector


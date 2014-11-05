;************************************************************************ 
;* HC12 Library: LCD_Lib.inc 						* 
;* Processor: MC9S12XDP512						* 
;* Xtal Speed: 16 MHz 							* 
;* Author: Brandon Foote 						* 
;* Date: October 14th, 2014 						* 
;* 							`		* 
;* Contains: LCD_Init 							* 
;* LCD_Ctrl 								* 
;* LCD_Busy 								* 
;* LCD_Char 								* 
;* LCD_Addr 								* 
;* LCD_Blink 								* 
;* LCD_NoBlink 								* 
;* LCD_CursOff 								* 
;* LCD_Clear								* 
;* LCD_Home 								* 
;* LCD_String 								* 
;* LCD_CharGen 								* 
;* LCD_CharGen8 							* 
;* 									* 
;************************************************************************ 

;************************************************************************
;*LCD_Init                                				*
;*									*
;*Registers affected: none						*
;*									*
;*Initializes the LCD display, Display on, Cursor on/increment 		*
;*mode, cursor set to home						*
;************************************************************************
LCD_Init: 
 	PSHD 				;save accumulator 
 	CLR 	PTH 			;ready to initialize 
 	MOVB 	#%11111111,DDRH 	;setup Port H to write to LCD
	CLR 	PORTK 			;/WRITE low, enable low, RS low 
 	MOVB 	#%00000111,DDRK 	;now make them outputs
	LDD 	#$FFFF 			;setup a 49.15 ms countdown 
NotYet1: 
	LBRN 	* 			;[3] 
 	DBNE 	D,NotYet1 		;[3] decrement, loop 

	MOVB	#%00111000,PTH	
	BSET	PORTK,%00000001		
	BCLR	PORTK,%00000111

	LDD	#11000
	DBNE	D,*

	BSET	PORTK,%00000001		
	BCLR	PORTK,%00000111

	LDD	#267
	DBNE	D,*

	BSET	PORTK,%00000001		
	BCLR	PORTK,%00000111

	LDAA	#%00001110		;Display on, cursor on, blinking off
	JSR	LCD_Ctrl
	LDAA	#%00000110		;increment cursor mode, no shift
	JSR	LCD_Ctrl
	LDAA	#%00000001		;Cursor to home
	JSR	LCD_Ctrl
	PULD
	RTS

;****************************************************************** 
;* LCD_Busy 							  * 
;*Regs affected: CCR (Carry) 					  *  
;*Reads busy flag at b7, returns Carry to reflect Busy 		  *  
;****************************************************************** 
 
LCD_Busy: 
	CLR 	DDRH 			;make inputs to read from LCD 
 	BSET 	PORTK,%00000011 	;Read high, enable high 
 	BCLR 	PORTK,%00000111 	;Read low, enable low (and RS, too) 
 	BRCLR 	PTH,%10000000,NotBusy 	;check busy flag, found in b7
	SEC
	BRA 	LeaveBusy
NotBusy:
	CLC
LeaveBusy:
	BSET	DDRH,#%11111111
	RTS
	
	
;****************************************************************** 
;*LCD_Ctrl 							  *  
;*Regs affected: None 						  *  
;*sends command received in Accumulator A to LCD 		  * 
;*This routine relies on LCD_Busy, which returns a Carry 	  * 
;****************************************************************** 
LCD_Ctrl: 
 	JSR 	LCD_Busy 		;wait until the LCD controller is free 
 	BCS 	LCD_Ctrl 		;-- as reported in Carry 
	
	STAA	PTH			;Put the command on the bus
	;MOVB	#%00000000,PORTK
	BSET	PORTK,%00000001		
	BCLR	PORTK,%00000111

	RTS
;************************************************************************
;*LCD_Char								*
;*									*
;*Registers affected: none						*
;*									*
;*Writes an ASCII character specified in A to the LCD display at	* 
;*the Cursor location							*
;************************************************************************

LCD_Char
	JSR 	LCD_Busy 		;wait until the LCD controller is free 
 	BCS 	LCD_Char 		;-- as reported in Carry 
	
	STAA	PTH			;Put the command on the bus
	;MOVB	#%00000100,PORTK
	BSET	PORTK,%00000101		
	BCLR	PORTK,%00000111

	RTS

;********************************************************
;*LCD_CharGen						*
;*							*
;*Regs affected: none					*
;*							*
;*Expects X to point to a table of eight line-by line	*
;*custom character generation bytes, to be placed in 	*
;*ASCII character indicated by A			*						
;********************************************************

LCD_CharGen:
	PSHA				;Save Accumulators
	PSHX
	PSHY
	
	LDY	#8			;For Countdown
	
	ANDA	#%00000111		;In case someone sent an invalid ASCII character
					;Multiply by eight to locate desired address
	LSLA
	LSLA
	LSLA

	ORAA	#%01000000		;instruction for CGRAM address mode as desired
	JSR	LCD_Ctrl
	
NextLineCG:
	LDAA	0,X			;Get character line definition
	ANDA	#%00111111		;in case invalid data has been entered
	JSR	LCD_Char		;reuse the routine to stuff CGRAM
	INX				;Next Line definition
	DEY				;countdown
	BNE	NextLineCG

	LDAA	#%10000000		;back to DDRAM, at home position
	JSR	LCD_Ctrl

	PULY
	PULX
	PULA				;Restore accumulator
	RTS		

;********************************************************
;*LCD_CharGen8						*		
;*							*
;*Registers affected: None				*
;*							*
;*Loads in a memory location in X, counts throuh 64 	*
;*locations using Y and sets the Ascii characters for 	*
;*them							*
;********************************************************

LCD_CharGen8:
	PSHA				;Save accumlators
	PSHX
	PSHY
	
	LDY	#64			;For Countdown
	
	LDAA	#%01000000		;instruction for CGRAM address mode, ASCII S
	JSR	LCD_Ctrl
	
NextLineCG8:
	LDAA	0,X			;Get character line definition
	ANDA	#%00111111		;in case invalid data has been entered
	JSR	LCD_Char		;reuse this routine to stuff CGRAM
	INX				;next line definition
	DEY				;countdown
	BNE	NextLineCG8
	
	LDAA	#%10000000		;back to DDRAM at home position
	JSR	LCD_Ctrl
	
	PULY
	PULX
	PULA				;restore accumulator
	RTS
;********************************************************
;*LCD_Addr						*
;*							*
;*Registers affected: None				*
;*							*
;*Reads in a location in A and sets the cursor position	*
;********************************************************
LCD_Addr:
	PSHA			;save accumulator
	ORAA	#%10000000	;makes command into "Set DDRAM address
	JSR	LCD_Ctrl	;Send instruction
	
	PULA			;restore accumulator
	RTS

;********************************************************
;*LCD_Blink						*
;*							*
;*Registers affected: none				*
;*							*
;*Display on, cursor on, Blink on			*
;********************************************************
LCD_Blink:
	PSHA			;Saves accumulator

	LDAA	#%00001111	;Display on, cursor on, blink on
	JSR	LCD_Ctrl	;send instruction
	
	PULA			;Restore accumulator
	RTS

;********************************************************
;*LCD_NoBlink
;*
;*Registers affescted: None
;*
;*Display on, Cursor on, Blink off
;********************************************************
LCD_NoBlink:
	PSHA

	LDAA	#%00001110
	JSR	LCD_Ctrl

	PULA
	RTS
;********************************************************
;*LCD_CursOff						*
;*							*
;*Registers affected: none				*
;*							*
;*Display on, cursor off				*
;********************************************************
LCD_CursOff:
  PSHA
  
  LDAA  #%00001100
  JSR   LCD_Ctrl
  
  PULA
  RTS

;********************************************************
;*LCD_String						*
;*							*
;*Registers affected: none				*
;*							*
;*Loads in a constant string location in X and writes 	*
;*it at location A					*
;********************************************************
LCD_String:
	PSHA
	PSHX
next_LCDChar:
	LDAA	0,X
	BEQ	null_Done	;on null character exit
	INX
	JSR	LCD_Char
	BRA	next_LCDChar
null_Done:
	PULX
	PULA
	RTS
;********************************************************
;*LCD_Home						*
;*							*
;*Registers affected: None				*
;*							*
;*Sets cusor location to home				*
;********************************************************
LCD_Home:
	PSHA

	LDAA	#%00000010
	JSR	LCD_Ctrl

	PULA
	RTS

;********************************************************
;*LCD_Clear						*
;*							*
;*Registers affected: None				*
;*							*
;*Clears the LCD display				*
;********************************************************
LCD_Clear
	PSHA

	LDAA	#%00000001
	JSR	LCD_Ctrl

	PULA
	RTS
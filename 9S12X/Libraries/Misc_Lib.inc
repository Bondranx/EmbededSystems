;************************************************************************ 
;*HC12 Program:Generally useful library components                      * 
;*Processor: MC9S12XDP512                                               * 
;*Xtal Speed: 16 MHz                                                    * 
;*Author: Brandon Foote                                                 * 
;*Date: Whenever                                                        * 
;*                                                                      * 
;*Details: Subroutines for general consumption:                         * 
;* BCD routines, text management,                                       * 
;* Timer initializations, delays, and interval management               * 
;*Contains:                                                             * 
;* Hex2BCD: converts numeric values to Binary Coded Decimal             * 
;*                                                                      * 
;************************************************************************ 



;****************************************************************** 
;* Hex2BCD                                                        * 
;*                                                                * 
;*Regs affected: A and B                                          * 
;*                                                                * 
;*A hexadecimal value arrives in Accumulator D and is converted   * 
;*to a 16-bit BCD, returned in D                                  * 
;*                                                                * 
;*Maximum hexadecimal value allowed is 270F                       * 
;*                                                                * 
;****************************************************************** 

Hex2BCD:
          PSHX
          LDX   #1000 
          IDIV
          
          XGDX
          LSLB
          LSLB
          LSLB
          LSLB
          PSHB
          XGDX
          LDX   #100
          IDIV
          XGDX
          PSHB
          XGDX 
          LDX   #10 
          IDIV         ;X contains nibble1, B contains nibble0 
          XGDX         ;ready to move result to upper nibble 
          LSLB 
          LSLB 
          LSLB 
          LSLB 
          PSHB
          XGDX 
          ORAB  0,SP   ;nibble0 with nibble1 in B 
          PULA         ;skip two eight-bit stackings 
          PULA 
          PULA         ;get nibble2 
          ORAA -2,SP   ;combine with nibble3
          PULX
          RTS
          
;************************************
;*A single hex digit arrives in A   *
;*converted to ASCII returned in A  *
;*No other registers changed        *
;************************************

Hex2Asc1:
		CMPA	#$0F
		BHI	LeaveHex2Asc1
		CMPA	#$0A
		BLO 	NotAlpha
		ADDA	#%00110111
		BRA	LeaveHex2Asc1
NotAlpha:
		CMPA	#9
		BHI	LeaveHex2Asc1
		ORAA	#%00110000
LeaveHex2Asc1:
		RTS
;***********
;*hex byte arrives in A
;*Two ascii characters are returned in D
;*
;************
Hex2Asc2:
		PSHA
		ANDA	#%00001111
		JSR	Hex2Asc1
		TFR	A,B
		PULA	
		LSRA
		LSRA
		LSRA
		LSRA
		JSR	Hex2Asc1
		RTS

;******************
;
;
;
;******************
AsciiToHex:
Loop:
	CMPA	#'0'
	BLO	Loop
	CMPA	#'9'
	BHI	Loop
	SUBA	#48
	RTS

;*******************************************
;* 
;*
;*
;*
;*******************************************
ToLower:
		CMPA	#'A'
		BLO	ExitToLower
		CMPA	#'Z'
		BHI	ExitToLower		
		ORAA	#%00100000
ExitToLower:	
		RTS	

;**********
;*
;*
;*
;**********

ToUpper:	
		CMPA	#'a'
		BLO	ExitToUpper
		CMPA	#'z'
		BHI	ExitToUpper		
		ANDA	#%11011111
ExitToUpper:	
		RTS

;************************************************
;*TimInit8us					*
;*Registers changed: none			*
;*						*
;*Sets up the time channel 0 for 8 us delays,	*
;*on output compare and toggles PT0		*
;************************************************
TimInit8us:
	MOVB	#%10000000,TSCR1	;enable timer mode
	MOVB	#%00000110,TSCR2	;set prescale to Bus/64 (8us per tick)
	MOVB	#%00000001,TIOS		;set IOS0 to output capture
	BSET	TFLG1,%00000001		;Clear the flag
	MOVB	#%00000001,TCTL2	;set PT0 to toggle mode
	RTS			
;************************************************
;*TimInit125ns					*
;*Registers changed: none			*
;*						*
;*Sets up the time channel 0 for 125 ns delays,	*
;*on output compare and toggles PT0		*
;************************************************
TimInit125ns:
	MOVB	#%10000000,TSCR1	;enable timer mode
	MOVB	#%00000000,TSCR2	;set prescale to Bus/1 (125ns per tick)
	MOVB	#%00000001,TIOS		;set IOS0 to output capture
	BSET	TFLG1,%00000001		;Clear the flag
	MOVB	#%00000001,TCTL2	;set PT0 to toggle mode
	RTS			
;************************************************
;*TimInit1us					*
;*Registers changed: none			*
;*						*
;*Sets up the time channel 0 for 1us delays,	*
;*on output compare and toggles PT0		*
;************************************************
TimInit1us:
	MOVB	#%10000000,TSCR1	;enable timer mode
	MOVB	#%00000011,TSCR2	;set prescale to Bus/8 (1us per tick)
	MOVB	#%00000001,TIOS		;set IOS0 to output capture
	BSET	TFLG1,%00000001		;Clear the flag
	MOVB	#%00000001,TCTL2	;set PT0 to toggle mode
	RTS			

;************************************************
;*Delay10ms
;*
;*Registers affected: none
;*
;*
;************************************************
Delay10ms:
	PSHD				;Store for later

	LDD	TCNT			;Get current clock setting
	ADDD	#1250			;10ms
	STD	TC0	
	
	BRCLR	TFLG1,%00000001,*	;block until delay is over
	
	BSET	TFLG1,%00000001		;clear flag
	
	PULD				;Restores old value for D
	RTS
;************************************************
;*Delay1ms
;*
;*Registers affected: none
;*
;*
;************************************************
Delay1ms:
	PSHD				;Store for later

	LDD	TCNT			;Get current clock setting
	ADDD	#125			;1ms
	STD	TC0	
	
	BRCLR	TFLG1,%00000001,*	;block until delay is over
	
	BSET	TFLG1,%00000001		;clear flag
	
	PULD				;Restores old value for D
	RTS

;****
;*
;*
;*
;****
Delay1us:
	PSHD				;Store for later

	LDD	TCNT			;Get current clock setting
	ADDD	#40			;1ms
	STD	TC0	
	
	BRCLR	TFLG1,%00000001,*	;block until delay is over
	
	BSET	TFLG1,%00000001		;clear flag
	
	PULD				;Restores old value for D
	RTS

;************************************************
;*DelayXms
;*
;*Registers affected: none
;*
;*
;************************************************
DelayXms:
	PSHX
	
LABEL:	JSR	Delay1ms
	DBNE	X,LABEL

	PULX
	RTS

;****************************************************************
;*DelayAms							*
;*								*
;*Registers changed: none					*
;*Must follow a TimeInit8us initialization, 8us/tick		*
;*This is a blocking routine					*
;*The number of milliseconds is brought in the A register	*
;*PT0 toggles after each delay
;****************************************************************
DelayAms:
	PSHD				;Store for later
	PSHA
	PSHB

	LDAB	#125			;1ms
	MUL				;A X B, result in D
	ADDD	TCNT			
	STD	TC0	
	
	BRCLR	TFLG1,%00000001,*	;block until delay is over
	BSET	TFLG1,%00000001		;clear flag
	
	PULB
	PULA
	PULD				;Restores old value for D
	RTS

;****************************************************************************************
;*IntXInitB										*
;*											*
;*Registers affected: none								*
;*Requires a prescaler value in B accumulator (Prescaler = 2^B) maximum B value = 7 for	*
;*/128											*
;*											*
;*Subroutine for setting up an interval timer using a value of X, prescaler in B for 	*
;*more flexibility									*
;****************************************************************************************

IntXInitB:
	PSHD
	PSHX
	
	BSET	TSCR1,%10000000		;enable timer module 0
	ANDB	#%00000111		;idiot-proofing
	STAB	TSCR2			;prescaler set to Bus/(2^B)
	BSET	TIOS,%00000001		;set IOS0 for output compare
	BSET	TCTL2,%00000001		;toggle mode for PT0
	BCLR	TCTL2,%00000010		;(continued)
	TFR 	X,D			;transfers interval in register X into register D
	ADDD 	TCNT			;Sets new event timer value, based on what used to be in X
	STD	TC0			;set the new target time
	BSET	TFLG1,#%00000001	;clears flag by setting bit to 1
	BRCLR	TFLG1,#%00000001,*	;first interval
		
	PULX
	PULD
	RTS

;************************************************
;*IntX						*
;*Registers affected: None			*
;*						*
;*Subroutine for managing an interval timer	*
;*using a value of X				*
;************************************************
IntX:
	PSHD
	PSHX

	TFR	X,D			;transfers interval in register X nto Register D
	ADDD	TC0			;set new event timer based on interval
	STD	TC0			;set the new target time
	BSET	TFLG1,#%00000001	;creat the flag by setting bit to 1
	
	PULX
	PULD
	RTS

;***********************************************
;*SetPT7
;*
;*
;*
;*
;***********************************************
SetPT7:
	
	BCLR	TIOS,%10000000		;Clears most signifigant bit ensiring input capture for IC7
	BCLR	TCTL3,%10000000		;Clears bit 7
	BSET	TCTL3,%01000000		;Sets bit 6 to 1

	RTS
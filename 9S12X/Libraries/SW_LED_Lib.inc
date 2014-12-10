;************************************************************************ 
;* HC12 Library: Switches and LEDs Library Components 			* 
;* Processor: MC9S12XDP512 						* 
;* Xtal Speed: 16 MHz 							* 
;* Author: Brandon Foote 						* 
;* Date: October 7, 2014						* 
;* 									* 
;* Details: Subroutines to initialize PT1AD1 to match CNT 		* 
;* development kit, to read and debounce the switches, 			* 
;* and turn on LEDs 							* 
;* 									* 
;* Routines: 								* 
;* 									* 
;* SW_LED_Init: initializes ports and registers for switches and LEDs 	* 
;* RedLEDon: turns Red LED on 						* 
;* RedLEDoff: turns Red LED off 					* 
;* YelLEDon: turns Yellow LED on 					* 
;* YelLEDoff: turns Yellow LED off 					* 
;* GrnLEDon: turns Green LED on 					* 
;* GrnLEDoff: turns Green Led off 					* 
;* SwCk: returns debounced condition of all Sw's in Acc A 		* 
;* ChkLeftSw: returns debounced condition of Left Sw as Carry 		* 
;* ChkMidSw: returns debounced condition of Mid Sw as Carry 		* 
;* ChkRtSw: returns debounced condition of Right Sw as Carry 		* 
;* ChkUpSw: returns debounced condition of Up Sw as Carry 		* 
;* ChkDnSw: returns debounced condition of Down Sw as Carry 		* 
;* 									* 
;************************************************************************ 

;********************************************************
;*SW_LED_Init						*
;*Turns the ports and resgisters for switches		*
;*and LEDs on						*
;*							*
;*Registers affected:	N/A				*
;********************************************************
SW_LED_Init:
	MOVB  	#%11100000,DDR1AD1
        MOVB  	#%00011111,ATD1DIEN1
	RTS

;********************************************************
;*RedLEDon						*
;*Turns on the red LED					*
;*							*
;*Registers affected:	N/A				*
;********************************************************
RedLEDon:
	BSET	PT1AD1,#%10000000
	RTS

;********************************************************
;*RedLEDoff						*
;*Turns off the red LED					*
;*							*
;*Registers affected:	N/A				*
;********************************************************
RedLEDoff:
	BCLR	PT1AD1,#%10000000
	RTS

;********************************************************
;*YelLEDon						*
;*Turns on the Yellow LED				*
;*							*
;*Registers affected:	N/A				*
;********************************************************
YelLEDon:
	BSET	PT1AD1,#%01000000
	RTS

;********************************************************
;*YelLEDoff						*
;*Turns off the Yellow LED				*
;*							*
;*Registers affected:	N/A				*
;********************************************************
YelLEDoff:
	BCLR	PT1AD1,#%01000000
	RTS

;********************************************************
;*GrnLEDon						*
;*Turns on the Green LED				*
;*							*
;*Registers affected:	N/A				*
;********************************************************
GrnLEDon:
	BSET 	PT1AD1,#%00100000
	RTS
	
;********************************************************
;*GrnLEDoff						*
;*Turns off the Green LED				*
;*							*
;*Registers affected:	N/A				*
;********************************************************
GrnLEDoff:
	BCLR	PT1AD1,#%00100000
	RTS
;********************************************************
;*SwCk							*
;*Checks current state of switches and if a change is 	*
;*detected returns the new state of the switches	*
;*							*
;*Registers affected:	A				*
;********************************************************
SwCk:
	PSHX
	PSHB
Loop0:
	LDAA	PT1AD1		;Loads accumulator A with current state of switches and LEDs
	ANDA	#%00011111	;Masks the LEDs to be off
	LDX     #26666   	;initializes a 10ms timer loop
        DBNE  	X,*		;loops through X for 10ms
	LDAB	PT1AD1		;Loads accumulator B with updated state of switches and LEDs
	ANDB	#%00011111	;Masks LEDs to be off	
	CBA			;Compares accumulator A and accumulator B to determine if switch state has changed
	BNE	Loop0		;Brances to start of routine if switch state is not the same
	PULB
	PULX
	RTS
;********************************************************
;*ChkLeftSw						*
;*Checks the current state of the Left switch		*
;*							*
;*Registers affected:	N/A				*
;********************************************************
ChkLeftSw:
	PSHA
	JSR	SwCk		;Returns all 5 switches
	ANDA	#%00001000	;Mask all other switches off
	BNE	SetCarryUp	;If switch is set, AND will not be zero
	CLC
	BRA	leaveup
SetCarryUp:
	SEC
leaveup:
	PULA
	RTS
;********************************************************
;*ChkMidSw						*
;*Checks the current state of the Middle switch		*
;*							*
;*Registers affected:	N/A				*
;********************************************************
ChkMidSw:
	PSHA
	JSR	SwCk		;Returns all 5 switches
	ANDA	#%00000001	;Mask all other switches off
	BNE	SetCarryUp2	;If switch is set, AND will not be zero
	CLC
	BRA	leaveup2
SetCarryUp2:
	SEC
leaveup2:
	PULA
	RTS
;********************************************************
;*ChkRtSw						*
;*Checks the current state of the right switch		*
;*							*
;*Registers affected:	N/A				*
;********************************************************
ChkRtSw:
	PSHA
	JSR	SwCk		;Returns all 5 switches
	ANDA	#%00000010	;Mask all other switches off
	BNE	SetCarryUp3	;If switch is set, AND will not be zero
	CLC
	BRA	leaveup3
SetCarryUp3:
	SEC
leaveup3:
	PULA
	RTS
	
;********************************************************
;*ChkUpSw						*
;*Checks the current state of the upper switch		*
;*							*
;*Registers affected:	N/A				*
;********************************************************
ChkUpSw:
	PSHA
	JSR	SwCk		;Returns all 5 switches
	ANDA 	#%00010000	;Mask all other switches off
	BNE	SetCarryUp4
	BRA	leaveup4
SetCarryUp4:
	SEC
leaveup4:
	PULA
	RTS

;********************************************************
;*ChkDnSw						*
;*Checks the current state of the Down switch		*
;*							*
;*Registers affected:	N/A				*
;********************************************************
ChkDnSw:
	PSHA
	JSR	SwCk		;Returns all 5 switches
	ANDA	#%00000100	;Mask all other switches off
	BNE	SetCarryUp5	;If switch is set, AND will not be zero
	CLC
	BRA	leaveup5
SetCarryUp5:
	SEC
leaveup5:
	PULA
	RTS
;****************************************************************** 
;* Title: Seven Segment Controller Library 			  * 
;* Processor: MC9S12XDP512 					  * 
;* Xtal Speed: 16 MHz 						  * 
;* Author: Brandon Foote						  * 
;* Date: Now 							  * 
;* 							 	  * 
;* Contains: SevSeg_Init 					  * 
;* SevSeg_Char 							  * 
;* SevSeg_dChar 						  * 
;* SevSeg_BlChar 						  * 
;* SevSeg_BlAll 						  * 
;* SevSeg_Two 							  * 
;* SevSeg_Top4 							  * 
;* SevSeg_Low4 							  * 
;* SevSeg_Eight 						  * 
;* SevSeg_Cust 							  * 
;* 								  * 
;****************************************************************** 

;****************************************************************** 
;* SevSeg_Init 							  * 
;* 								  * 
;*Regs affected: none 						  * 
;* 								  * 
;*Sets up Port A for the Seven Segment Controller as control, 	  * 
;*Sets up Port B for the Seven Segment Controller as data 	  * 
;* *only b1 and b0 of Port A are used 				  * 
;* *clears all eight digits 					  * 
;* 								  * 
;****************************************************************** 

SevSeg_Init: 	BSET 	PORTA,%00000011 	;preset A0:1 to HIGH 
 		MOVB 	#%11111111,PORTB 	;preset all PORTB HIGH 

 		BSET 	DDRA,%00000011 		;make A0:1 outputs 
 		MOVB 	#%11111111,DDRB 	;make all PORTB outputs
		JSR 	SevSeg_BlAll 
 		RTS


;****************************************************************** 
;* SevSeg_Char 							  * 
;* 								  * 
;* Regs affected: none 						  * 
;* 								  * 
;* Accepts a hex character in Accumulator A and 		  * 
;* a location (0 to 7) in Accumulator B and places the character  * 
;* 								  * 
;* The routine expects the user to know the LED limits, 	  * 
;* and assumes that SevSeg_Init has been run already. 		  * 
;* 								  * 
;****************************************************************** 
SevSeg_Char: 	PSHA 
 		PSHB 

 		ANDB 	#%00000111 		;clean up address 
 		ORAB 	#%01011000 		;add control: hex, decode, no SD, Bnk A 
 		ANDA 	#%00001111 		;clear upper nibble 
 		ORAA 	#%10000000 		;no decimal point 

 		STAB 	PORTB 			;instruction ready 
 		MOVB 	#%00000010,PORTA 	;/mode high, write low 
 		MOVB 	#%00000011,PORTA 	;/write back to high 

 		STAA 	PORTB 			;data ready 
 		MOVB 	#%00000000,PORTA 	;mode and /write low 
 		MOVB 	#%00000011,PORTA 	;mode and /write back to high 

 		PULB 
 		PULA 
 		RTS 

;****************************************************************** 
;* SevSeg_dChar 						  * 
;* 								  * 
;* Regs affected: none 						  * 
;* 								  * 
;* Accepts a hex character in Accumulator A and 		  * 
;* a location (0 to 7) in Accumulator B and places the character  * 
;* followed by a decimal point 					  * 
;* 								  * 
;* The routine expects the user to know the LED limits, 	  * 
;* and assumes that SevSeg_Init has been run already. 		  * 
;* 								  * 
;****************************************************************** 
SevSeg_dChar:	PSHA 
 		PSHB 

 		ANDB 	#%00000111 		;clean up address 
 		ORAB 	#%01011000 		;add control: hex, decode, no SD, Bnk A 
 		ANDA 	#%00001111 		;clear upper nibble  

 		STAB 	PORTB 			;instruction ready 
 		MOVB 	#%00000010,PORTA 	;/mode high, write low 
 		MOVB 	#%00000011,PORTA 	;/write back to high 

 		STAA 	PORTB 			;data ready 
 		MOVB 	#%00000000,PORTA 	;mode and /write low 
 		MOVB 	#%00000011,PORTA 	;mode and /write back to high 

 		PULB 
 		PULA 
 		RTS 


;****************************************************************** 
;* SevSeg_BlChar 						  * 
;* 								  * 
;* Regs affected: none 						  * 
;* 								  * 
;* Accepts a location (0 to 7) in Accumulator B and blanks 	  * 
;* that location. 						  * 
;* 								  * 
;****************************************************************** 

SevSeg_BlChar:	PSHA 
 		PSHB 

 		ANDB 	#%00000111 		;clean up address 
 		ORAB 	#%01111000 		;add control: hex, decode, no SD, Bnk A 
		LDAA 	#%10000000		;all segments blank  
 

 		STAB 	PORTB 			;instruction ready 
 		MOVB 	#%00000010,PORTA 	;/mode high, write low 
 		MOVB 	#%00000011,PORTA 	;/write back to high 

 		STAA 	PORTB 			;data ready 
 		MOVB 	#%00000000,PORTA 	;mode and /write low 
 		MOVB 	#%00000011,PORTA 	;mode and /write back to high 

 		PULB 
 		PULA 
 		RTS 


;****************************************************************** 
;* SevSeg_BlAll 						  * 
;* 								  * 
;* Regs affected: none  					  * 
;* 								  * 
;* Blanks all eight digits. 					  * 
;* 								  * 
;****************************************************************** 

SevSeg_BlAll:
		PSHA 
 		PSHB
	   	JSR SevSeg_Init
           	LDAA #%10000000
           	LDAB #00
nextBl:    	JSR SevSeg_BlChar
           	INCB 
           	CMPB #8
           	BNE nextBl
           	PULB
		PULa
		RTS
			
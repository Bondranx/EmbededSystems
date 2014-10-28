;************************************************************************ 
;* HC12 Library: Serial COmunication Library 				* 
;* Processor: MC9S12XDP512 						* 
;* Xtal Speed: 16 MHz 							* 
;* Author: Brandon Foote						* 
;* Date: October 7, 2014						* 
;* 									* 
;* Details: Subroutines to initailize and read from the serial 		*
;*	    communication port						*
;* 									* 
;* Routines: 								*
;*	SCI0Init192							* 
;* 	SCI0_TxByte							* 
;* 	SCI0_RxByte							*
;* 	SCI0_TxStr							* 
;************************************************************************ 

;************************************************************************
;* SCI0_Init192								* 
;* Initializes the communications port setting baud rate 		*
;*and transfer types 							* 
;* Registers affected: none 						* 
;************************************************************************ 
SCI0Init192:
		movw	#26, SCI0BD	;Sets Baud Rate to 19.2 kb/s
		movb	#%00000000,SCI0CR1
		movb	#%00001100,SCI0CR2
		RTS

;************************************************************************
;* SCI0_TxByte 								* 
;* Sends a byte brought in accumulator A to SCI0, and waits until 	* 
;* the transmit buffer is empty before returning (blocking routine) 	* 
;* Registers affected: none 						* 
;************************************************************************ 
SCI0_TxByte: 
		TST SCI0SR1 
 		BPL SCI0_TxByte ;Loops if there is not data in the SCI buffer
 		STAA SCI0DRL 	;Load s abyte from SCI port to accumulator A
 		RTS 

;************************************************************************
;* SCI0_RxByte 								* 
;* Receives a byte in accumulator A from SCI0 if a valid byte is 	* 
;* present. The presence or absence of a valid byte is indicated 	* 
;* using the Carry bit as a flag. 					* 
;* Registers affected: A 						* 
;************************************************************************ 
SCI0_RxByte:
		BRCLR 	SCI0SR1,%00100000,SCI0RxNot ;Branches to clear C if all bits are clear
 		LDAA 	SCI0DRL 	;Load accumulator A with a byte from the SCI port
 		SEC 			
 		BRA 	SCI0_RxEnd	;Branch to end program without clearing C 	
SCI0RxNot: 	CLC 
		
SCI0_RxEnd: 	RTS 

;************************************************************************
;SCI0_TxStr								*
;Regs affected: none							*
;Index Reg X brings address of null-terminated string			*
;Relies on SCI0_TxByte Subroutine					*
;************************************************************************
SCI0_TxStr:
		PSHA
		PSHX
next_txchar:
		LDAA	0,x
		BEQ	null_done	;on null character exit
		JSR	SCI0_TxByte
		INX	
		bra	next_txchar
		
null_done	PULX
		PULA
		RTS	
		

		
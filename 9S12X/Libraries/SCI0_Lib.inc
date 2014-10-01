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
 		BPL SCI0_TxByte 
 		STAA SCI0DRL 
 		RTS 

;************************************************************************
;* SCI0_RxByte * 
;* Receives a byte in accumulator A from SCI0 if a valid byte is * 
;* present. The presence or absence of a valid byte is indicated * 
;* using the Carry bit as a flag. * 
;* Registers affected: A * 
;************************************************************************ 
SCI0_RxByte:
		BRCLR 	SCI0SR1,%00100000,SCI0RxNot 
 		LDAA 	SCI0DRL 
 		SEC 
 		BRA 	SCI0_RxEnd 
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
next_tchar:
		LDAA	0,x
		BEQ	null_done	;on null character exit
		JSR	SCI0_TxByte
		INX	
		bra	next_txchar
		
null_done	PULX
		PULA
		RTS	
		

		
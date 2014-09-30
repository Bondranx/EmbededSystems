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
;*Maximum hexadecimal value allowed is ____                       * 
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
          PULA          ;skip two eight-bit stackings 
          PULA 
          PULA         ;get nibble2 
          ORAA -2,SP   ;combine with nibble3
          PULX
          RTS
          
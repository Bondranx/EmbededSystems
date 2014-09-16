;*****************************************************************
;* This stationery serves as the framework for a                 *
;* user application (single file, absolute assembly application) *
;* For a more comprehensive program that                         *
;* demonstrates the more advanced functionality of this          *
;* processor, please see the demonstration applications          *
;* located in the examples subdirectory of the                   *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*****************************************************************

; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point



; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data

; variable/data section

            ORG RAMStart
 ; Insert here your data definition.
Counter     DS.W 1
FiboRes     DS.W 1


; code section
            ORG   ROMStart


Entry:
_Startup:
            LDS   #RAMEnd+1       ; initialize the stack pointer

            CLI                     ; enable interrupts

            CLR   PT1AD1
            MOVB  #%11100000,DDR1AD1
            BSET  ATD1DIEN1,%00011111

Loop        LDAA PT1AD1
            BCLR PT1AD1,%10000000 
            BCLR PT1AD1,%00100000
            BCLR PT1AD1,%01000000
            BRSET PT1AD1,%00010000,UP            
LFT:        BRSET PT1AD1,%00001000,LEFT
RGHT:       BRSET PT1AD1,%00000010,RIGHT
MIDD:       BRSET PT1AD1,%00000001,MID
            BRA Loop
            
UP:         
Loop2:      BSET PT1AD1,%11100000
            BRSET PT1AD1,%00000100, DOWN
            BRA Loop2
                                   
LEFT:       BSET PT1AD1,%10000000
            ;JSR Timer2s           
            BRA RGHT
RIGHT:      BSET PT1AD1,%00100000
            ;JSR Timer2s
            BRA MIDD
MID:        BSET PT1AD1,%01000000
            ;JSR Timer2s
            BRA Loop
DOWN:       BSET PT1AD1,%11100000     
            BRA Loop
Timer2s:
           LDAB   #89
Timloop:   LDY    #59925
           DBNE   Y,*
           DBNE   B,Timloop
           RTS              
;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector

%%file project.asm 
;;306508400520000000087000031003010080900863005050090600130000250000000074005206300
;;R6 AND R7 ARE STACK
;;R5 is current location
;;R1 is num (we are looking for) in asscii

.ORIG x3000
    LEA R6, STACK         ;; R6 is Top of STACK
    LEA R0, START         ;; GOTO START
    JMP R0           ;; Keep STACK close to reference
STACK:  
    .BLKW #200            ;; adjust size for your use

START:
    JSR GETINPUT          ;; JUMP TO GET INPUT
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; Here we display the board
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    LD R0, BOARD12        ;; LOADS BOARD
    ;PUTS                  ;; DISPLAYS BOARD 
    
    
    JSR JORDANS_SADNESS
    ;JSR ISSAFE
    ;LD R2, BOARD12       ;; LOADS BOARD1
    ;JSR STARTFINDZEROLOCATION ;; FINDS THE FIRST ZERO AND SAVES IT IN R5 IF R5 IS 0 THEN THERE IS NO ZEROS
    ;LD R1, NONE
    ;AND R2,R2, #0
    ;JSR ISSAFE
    NEWLINEHTML .STRINGZ "\n"
    LD R2, NEWLINEHTML ;;SPACE ;;R0? R2>R1>R0 VS LEA
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; This is the end of the program. First we print the board and then end
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
END:
    JSR BEGINFINALPRINTBOARD
    HALT

;NEWLINEHTML2 .STRINGZ "\n"
;FINALUGH .BLKW 100
;FINALUGH2 .FILL FINALUGH
BEGINFINALPRINTBOARD:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    LD R2, BOARD12
    AND R3, R3, #0 ;;CREATE COUNTER
    ADD R3, R3, #10 ;;SET W/ 9
NEWLINE:
    AND R4, R4, #0 ;;CREATE COUNTER
    ADD R4, R4, #9 ;;SET W/ 9
    LEA R0, ENTER
    PUTS
    ADD R3, R3, #-1
    BRz ENDFINALPRINTBOARD
FINALPRINTBOARD:
    LDR R0, R2, #0
    OUT
    ADD R2, R2, #1
    ADD R4, R4, #-1 ;;COUNTER GO DOWN
    BRp FINALPRINTBOARD
    BRz NEWLINE
ENDFINALPRINTBOARD:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
NNINE10 .FILL xFFC7
KEEPINGTRACKARR: .FILL x0000
ENTER .STRINGZ "\n"
E1 .FILL x0051
NE1 .FILL xFFAF
JORDANS_SADNESS:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    LEA R0, ENTER
    PUTS
    LD R0, BOARD12
    PUTS
    LD R0, BOARD12
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;Find location of zero
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    LD R2, BOARD12       ;; LOADS BOARD1
    JSR STARTFINDZEROLOCATION ;; FINDS THE FIRST ZERO AND SAVES IT IN R5 IF R5 IS 0 THEN THERE IS NO ZEROS
    
    LDI R4, TOPSTACKLOC2
    STR R5, R4, #0
    ADD R4, R4, #1
    STI R4, TOPSTACKLOC2
    
    
    ADD R5, R5, #0       ;; IF ZERO +R5 IS ZERO THEN THERE ARE NO ZEROS LEFT AND THE PROGRAM SHOULD END
    BRnp DOONE
    
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR1
    
    ;LDI R4, TOPSTACKLOC2
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC2
    
 
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    

STACKLOC2: .FILL STACKLOC
TOPSTACKLOC2: .FILL TOPSTACKLOC
NONE .FILL xFFCF
NTWO .FILL xFFCE
NTHREE .FILL xFFCD
NFOUR .FILL xFFCC
NFIVE .FILL xFFCB

ZERO .FILL x0030
ONE .FILL x0031
TWO .FILL x0032
THREE .FILL x0033
FOUR .FILL x0034
FIVE .FILL x0035
 
BOARD12: .FILL BOARD1
VAR1: .FILL VAR
ZERO1: .FILL x0030            ;; THE ASCII OF ZERO

DOONE:  
    
    LD R1, NONE
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DOTWO         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ONE
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC2
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR1
    ADD R0, R0, #0
    BRnz REMOVE1
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR1
    
    ;LDI R4, TOPSTACKLOC2
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC2
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE1:
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO1
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION


DOTWO:  
    
    LD R1, NTWO
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DOTHREE         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, TWO
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC2
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR1
    ADD R0, R0, #0
    BRnz REMOVE2
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR1
    
    ;LDI R4, TOPSTACKLOC2
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC2
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE2:
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO1
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    
DOTHREE: 
    
    LD R1, NTHREE
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DOFOUR         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, THREE
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC2
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR1
    ADD R0, R0, #0
    BRnz REMOVE3
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR1
    
    ;LDI R4, TOPSTACKLOC2
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC2
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE3:
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO1
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    
DOFOUR:
    
    LD R1, NFOUR
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DOFIVE         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, FOUR
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC2
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR1
    ADD R0, R0, #0
    BRnz REMOVE4
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR1
   
    ;LDI R4, TOPSTACKLOC2
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC2
    
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE4:
    LD R0, BOARD12     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO1
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    
DOFIVE:
   
    LD R1, NFIVE
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DOSIX        ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD12    ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, FIVE
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC3
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR1
    ADD R0, R0, #0
    BRnz REMOVE5
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR1
    
    ;LDI R4, TOPSTACKLOC3
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC3
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE5:
    LD R0, BOARD13    ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO1
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
DOSIX:
   
    LD R1, NSIX
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DOSEVEN         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD13     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, SIX
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC3
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR2
    ADD R0, R0, #0
    BRnz REMOVE6
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR2
    
    ;LDI R4, TOPSTACKLOC3
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC3
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE6:
    LD R0, BOARD13     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO2
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
DOSEVEN:
    
    LD R1, NSEVEN
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DOEIGHT         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD13     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, SEVEN
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC3
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR2
    ADD R0, R0, #0
    BRnz REMOVE7
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR2
    
    ;LDI R4, TOPSTACKLOC3
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC3
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE7:
    LD R0, BOARD13     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO2
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
DOEIGHT:
   
    LD R1, NEIGHT
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp DONINE         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD13     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, EIGHT
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC3
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR2
    ADD R0, R0, #0
    BRnz REMOVE8
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR2
   
    ;LDI R4, TOPSTACKLOC3
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC3
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE8:
    LD R0, BOARD13     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO2
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION

DONINE:
    
    LD R1, NNINE
    AND R2,R2, #0
    JSR ISSAFE
    ADD R2, R2, #0
    BRnp FINISHLOOP         ;;IF NOT POSITIVE SKIP ADDING VALUE TO BOARD AND RECURSION
    LD R0, BOARD13    ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, NINE
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION
    JSR JORDANS_SADNESS
    LDI R4, TOPSTACKLOC3
    ADD R4, R4, #-1
    LDR R5, R4, #0
    LDI R0, VAR2
    ADD R0, R0, #0
    BRnz REMOVE9
    
    ;SET VAR TO TRUE
    AND R0, R0, #0
    ADD R0, R0, #1
    STI R0, VAR2
   
    ;LDI R4, TOPSTACKLOC3
    ;ADD R4, R4, #-1
    ;LDR R5, R4, #0
    ;STI R4, TOPSTACKLOC3
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
REMOVE9:
    LD R0, BOARD13     ;;GET BOARD
    ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ADD R0, R0, #-1    ;;OFF BY 1
    LD R7, ZERO2
    STR R7, R0, #0     ;;STORE VALUE IN LOCATION


FINISHLOOP:
    ;SET VAR TO False
    AND R0, R0, #0
    STI R0, VAR2
    
    LDI R4, TOPSTACKLOC3
    ADD R4, R4, #-1
    LDR R5, R4, #0
    STI R4, TOPSTACKLOC3
    
    ;LD R0, BOARD13     ;;GET BOARD
    ;ADD R0, R0, R5     ;;ADD LOCATION TO BOARD
    ;ADD R0, R0, #-1    ;;OFF BY 1
    ;LD R7, ZERO2
    ;STR R7, R0, #0     ;;STORE VALUE IN LOCATION

    
    
    ;;POP ACTUALSTACK
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
E11 .FILL x0051
NE12 .FILL xFFAF
BOARD13: .FILL BOARD1

STACKLOC3: .FILL STACKLOC
TOPSTACKLOC3: .FILL TOPSTACKLOC
VAR2: .FILL VAR
VAR: .FILL x0000
NSIX .FILL xFFCA
NSEVEN .FILL xFFC9
NEIGHT .FILL xFFC8
NNINE .FILL xFFC7
SIX .FILL x0036
SEVEN .FILL x0037
EIGHT .FILL x0038
NINE .FILL x0039 

ZERO2: .FILL x0030            ;; THE ASCII OF ZERO
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; NEEDS A NEGATED ASCII NUMBER IN R1 AND LOCATION IN R5
    ;; R2 WILL BE ZERO IF SAFE
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
ISSAFE:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    JSR STARTUSEDINROW
    JSR STARTUSEDINCOLUMN
    JSR STARTUSEDINBOX 
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
   
   
   
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; THIS METHOD WILL ADD A POSITIVE NUMBER TO R2 IF THE NUMBER IS USED IN COLUMN
    ;; TO USE THIS A NEGATED NUMBER MUST BE IN R1
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
     
    


STARTUSEDINBOX:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    JSR STARTFINDTOPOFBOX
    BR USEDINBOX
    
USEDINBOX:
    ;;R0 IS LOCATION OF TOP OF BOX
    LD R3, BOARD11
    ADD R3, R3, R0
    ADD R3, R3, #-1 ;;OFF BY 1
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #1
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #1
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #7
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #1
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #1
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #7
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #1
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    ADD R3, R3, #1
    LDR R4, R3, #0
    ADD R7, R4, R1
    BRz POSUSEDINBOX
    BR ENDUSEDINBOX
    
POSUSEDINBOX:
    ADD R2, R2, #1
    BR ENDUSEDINBOX
ENDUSEDINBOX:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET
    
    
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;DIVIDE R0 BY R3
    ;;R0 = R4
    ;;R1 = R3
    ;;R2 = R0
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
STARTDIVIDE2:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    ;clear reg 0 reg 1 and reg 2
    AND R4, R4, #0
    ;Adding the negative version of what you are deviding 
    
    NOT R3, R3
    ADD R3, R3, #1
    
DIVIDE2:
    ;Add 1 to reg 4
    ADD R4, R4, #1
    ;Add reg 3 to reg 0 store in reg 0
    ADD R0, R0, R3
    ;check if need to loop
    BRp DIVIDE2
    
    
ENDDIVIDE2:
    ;ADD R4, R4, #-1
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 
    
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;R0 IS TOP OF BOX
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STARTFINDTOPOFBOX:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    
FINDTOPOFBOX:
    JSR STARTGETROWANDCOL   
    ;;;;;; NEED TO GET TROW AND TCOL
    LD R0, ROW
    AND R3, R3, #0         ;;CLEAR R3
    ADD R3, R3, #3         ;;ADD 3 TO R3
    JSR STARTDIVIDE2
    ST R4, TROW

    LD R0, COL
    AND R3, R3, #0         ;;CLEAR R3
    ADD R3, R3, #3         ;;ADD 3 TO R3
    JSR STARTDIVIDE2
    ST R4, TCOL
    
    LD R0, TROW
    
    ;; (9((ROW-1)x3)+((C0L-1)x3)+1   WILL GET US THE LOCATION OF THE TOP OF THE BOX
    ADD R0, R0, #-1
    JSR MULTIPLY9
    AND R7, R7, #0
    ADD R7, R7, R3
    ADD R7, R7, R3
    ADD R7, R7, R3
    ADD R3, R7, #0
    LD R0, TCOL
    
    
    ADD R0, R0, #-1
    AND R7, R7, #0
    ADD R7, R7, R0
    ADD R7, R7, R0
    ADD R7, R7, R0
    ADD R7, R7, #1
    ADD R0, R7, R3   
    
    BR ENDFINDTOPOFBOX
    
    
ENDFINDTOPOFBOX:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 
MULTIPLY9: 
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    AND R3, R3, #0         ;; CLEAR R3
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R3, R3, R0
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 

    
BOARD11: .FILL BOARD1         
BOARD21: .FILL BOARD2
BOARD31: .FILL BOARD3
BOARD41: .FILL BOARD4
BOARD51: .FILL BOARD5
BOARD61: .FILL BOARD6
BOARD71: .FILL BOARD7
BOARD81: .FILL BOARD8
BOARD91: .FILL BOARD9    ;;MADE THIS ONE 20 SO THERE WOULD BE A NULL CHARACTER SO THAT THE PROGRAM WOULD KNOW WHEN TO STOP    
    
COL: .FILL x0000
ROW: .FILL x0000
TCOL: .FILL x0000
TROW: .FILL x0000
COLPART: .FILL x0000
ROWPART: .FILL x0000
STARTGETROWANDCOL:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    AND R0, R0, #0         ;;CLEAR R0 FOR COUNTER
    ADD R4, R5, #0         ;;PUT LOCATION IN R4
GETROWANDCOL:
    ADD R0, R0, #1          ;;ADD 1 TO COUNTER
    ADD R4, R4, #-9         ;;SUBTRACT 9 FROM LOCATION
    BRnz ENDGETROWANDCOL 
    BRp GETROWANDCOL
ENDGETROWANDCOL:
    ADD R4, R4, #9
    ST R4, COL
    ST R0, ROW
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 

STARTUSEDINCOLUMN:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    JSR STARTGETFIRSTINCOLUMN  ;; R4 WILL NOW HAVE THE FIRST POSITION OF THE COLUMN
    ;ADD R0, R4, #0
    ;PUTS
    LD R0, BOARD11
    ADD R0, R0, R4          ;;ADD R4 TO R0 TO GET STARTING POINT OF COLUMN
    ;PUTS
    AND R4, R4, #0          ;;CLEAR A COUNTER AND SET IT TO 9
    ADD R4, R4, #9
    
   
USEDINCOLUMN:
    LDR R3, R0, #0          ;; LOAD VALUE IN R0 INTO R3
    ADD R7, R3, R1          ;; COMPARE R1 WITH VALUE IN R3
    BRz ENDUSEDINCOLUMNP
    ADD R0, R0, #9          ;; GET POSITION OF NEXT VALUE IN COLUMN
    ADD R4, R4, #-1         ;; SUBTRACT 1 FROM COUNTER
    BRnz ENDUSEDINCOLUMN     ;; CHECK IF COUNTER IS ZERO
    BRp USEDINCOLUMN        ;; IF COUNTER IS ZERO END USED IN COLUMN
ENDUSEDINCOLUMN:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 
    
ENDUSEDINCOLUMNP:
    ADD R2, R2, #1      ;; Add 1 to r2 before einding 
    BR ENDUSEDINCOLUMN
    
STARTGETFIRSTINCOLUMN:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    ADD R4, R5, #0         ;; PUT THE LOCATION INTO R4
GETFIRSTINCOLUMN:
    ADD R4, R4, #-9
    BRp GETFIRSTINCOLUMN
    BRnz ENDGETFIRSTINCOLUMN
ENDGETFIRSTINCOLUMN:
    ADD R4, R4, #8         ;; WILL HAVE SUBTRACTED 1 TOO MANY SO ADD BACK ONE ROW
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; THIS CHECKS IF THE VALUE IS USED IN THE ROW IF THE VAULE IS USED R2 WILL BE POSITIVE OTHERWISE R2 IS ZERO
    ;; TO USE THIS A NEGATED NUMBER MUST BE IN R1
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


STARTUSEDINROW:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    JSR STARTDIVIDE
    JSR STARTFINDROW
    AND R2, R2, #0          ;;CLEAR R2
       
USEDINROW:
    ;;R0 BOARD
    ;;R1 IS NUMBER (NEGATED)
    ;;R2 IS COUNTER
    ;;R3 IS VALUE IN TABLE THAT WE ARE COMPAREING THE NUMBER TO
    LDR R3, R0, #0          ;; LOAD VALUE IN R0 INTO R3
    ADD R0, R0, #1          ;;INCREMENT R0
    ADD R2, R2, #1          ;;ADD ONE TO R2
    ADD R7, R3, R1          ;;COMPARE R1 WITH VALUE IN R3
    BRz ENDUSEDINROW        ;;IF VALUE IS SAME GO TO ENDUSEDINROW
    ADD R7, R2, #-9    ;;IF R2 IS 10 SET R2 TO ZERO AND GO TO EDNUSED IN ROW
    BRz ENDUSEDINROWZERO
    BR USEDINROW            ;;ELSE LOOP TO USEDINROW
ENDUSEDINROW:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 
    
ENDUSEDINROWZERO:
    AND R2, R2, #0
    BR ENDUSEDINROW
    
STARTFINDROW:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    
FINDROW:
    LEA R0, BOARD1
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD2
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD3
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD4
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD5
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD6
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD7
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD8
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R4, BOARD9
    ADD R4, R4, #-1
    BRz ENDFINDROW
    LEA R0, BOARD9           ;;SHOULD NOT BE CALLED
    BR ENDFINDROW
    
ENDFINDROW:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;; THIS DIVIDES LOCATION BY 9 SAVES RESULT IN R4
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STARTDIVIDE:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    
    LD R2, NNINE2           ;; LOAD VALUE 9 INT R1
   
    AND R4, R4, #0        ;; CLEAR R4 AGAIN
    AND R3, R3, #0         ;; CLEAR R3
    ADD R3, R5, #0         ;;PUT R5 IN R3 SO WE CAN CHANGE IT
DIVIDE:
    ADD R4, R4, #1         ;;ADD 1 TO R4
    ADD R3, R3, R2        ;;SUBTRACT 9 FROM R3
    BRp DIVIDE
    BRn ENDDIVIDE
    BRz ZENDDIVIDE
    
ZENDDIVIDE:
    ;ADD R4, R4, #-1
    BR ENDDIVIDE

ENDDIVIDE:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 
    
    
GETINPUT:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;This section gets user input for all 9 rows 1 row at a time
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    LEA R0, MESSAGE       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LD R1, RT             ;; LOADS THE RETURN CHARACTER
    JSR NEGATE            ;; NEGATES THE RETURN CHARACTER
    
    LEA R2, BOARD1        ;; LOADS BOARD1
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2      ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD2        ;; LOADS BOARD2
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD3        ;; LOADS BOARD3
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD4        ;; LOADS BOARD4
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD5        ;; LOADS BOARD5
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD6        ;; LOADS BOARD6
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD7        ;; LOADS BOARD7
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD8        ;; LOADS BOARD8
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    
    LEA R0, MESSAGE2       ;; GETS MESSAGE
    PUTS                  ;; DISPLAYS MESSAGE
    LEA R2, BOARD9        ;; LOADS BOARD9
    JSR GETBOARD          ;; JUMPS TO GET BOARD
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 


NEGATE:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    NOT R1, R1
    ADD R1, R1, #1
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET  
GETBOARD:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    GETC                   ;; read character
    ADD R3,R0,R1           ;; quit if character = return
    BRz ENDGETBOARD
    STR R0,R2, #0           ;; store character in the array
    ADD R2,R2, #1          ;; increment the address of the array cell
    BR GETBOARD             ;;read another character 
ENDGETBOARD:
    ;;STR R3,R2, #0          ;;store the null character after the last input
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET 
    
STARTFINDZEROLOCATION:
    STR R7, R6, #0         ;; Save R7 to Stack
    ADD R6, R6, #1         ;; Increment Stack 
    AND R5, R5, #0         ;; CLEAR R5
    
    BR FINDZEROLOCATION
FINDZEROLOCATION:
    LDR R0, R2, #0          ;; LOAD VALUE IN R2 INTO R0
    ADD R2, R2, #1         ;; ADD 1 TO R2
    ADD R5, R5, #1         ;; ADD 1 TO R5 THE LOCATION
    LD R1, NE1B             ;; LOADS THE NULL CHARACTER
    ADD R1, R1, #-1
    ;;JSR NEGATE            ;; NEGATES THE NULL CHARACTER 
    ADD R3, R5, R1         ;;IF STRING = NULL END
    BRz ENDFINDZEROLOCATIONZ
    LD R1, ZERO4             ;; LOADS THE ZERO CHARACTER
    JSR NEGATE            ;; NEGATES THE ZERO CHARACTER
    ADD R3, R0, R1        ;; IF VLUE DOEES NOT EQUAL STRING ZERO LOOP
    BRnp FINDZEROLOCATION
    BRz ENDFINDZEROLOCATION   ;; ELSE END 
ENDFINDZEROLOCATION:
    ADD R6, R6, #-1        ;; Decrement Stack
    LDR R7, R6, #0         ;; Load Stack into R7
    RET   
    
ENDFINDZEROLOCATIONZ:
    AND R5, R5, #0
    BR ENDFINDZEROLOCATION

    

BOARD1: .BLKW 9           ;;SINCE THERE IS NO SPACE IN BETWEEN IF YOU LOAD BOARD 1 YOU WILL LOAD THE ENTIRE BOARD :)
BOARD2: .BLKW 9
BOARD3: .BLKW 9
BOARD4: .BLKW 9
BOARD5: .BLKW 9
BOARD6: .BLKW 9
BOARD7: .BLKW 9
BOARD8: .BLKW 9
BOARD9: .BLKW 20          ;;MADE THIS ONE 20 SO THERE WOULD BE A NULL CHARACTER SO THAT THE PROGRAM WOULD KNOW WHEN TO STOP
    
    

RT: .FILL x000A              ;;the return character'
NE1B: .FILL xFFAF
NULL: .FILL x0000            ;; THE ASCII OF NULL
ZERO4: .FILL x0030            ;; THE ASCII OF ZERO

NTEN:  .FILL xFFF6


NNINE2: .FILL xFFF7          ;; HEX FOR -9
MESSAGE:  .STRINGZ "ENTER THE FIRST ROW OF THE BOARD (WITH 0'S FOR BLANKS):\n"
MESSAGE2:  .STRINGZ "ENTER THE NEXT ROW OF THE BOARD (WITH 0'S FOR BLANKS):\n"

STACKLOC: .BLKW #200
TOPSTACKLOC: .FILL STACKLOC

    .END



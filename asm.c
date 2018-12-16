#pragma asm

MOV A,#38H // Use 2 lines and 5x7 matrix
ACALL CMND
MOV A,#0FH // LCD ON, cursor ON, cursor blinking ON
ACALL CMND
MOV A,#01H //Clear screen
ACALL CMND
MOV A,#06H //Increment cursor
ACALL CMND
MOV A,#82H
ACALL CMND
MOV A,#3CH //Activate second line
ACALL CMND
MOV A,#49D
ACALL DISP
MOV A,#54D
ACALL DISP
MOV A,#88D
ACALL DISP
MOV A,#50D
ACALL DISP
MOV A,#32D
ACALL DISP
MOV A,#76D
ACALL DISP

#pragma endasm
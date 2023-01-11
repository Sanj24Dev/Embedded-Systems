	AREA RESET, CODE, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0X10001000
	DCD Reset_Handler
	ALIGN
	AREA MYCODE, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=DEST
	LDR R5,=COUNT
	LDR R5,[R5]
	MOV R1,#0
	MOV R2,#1
	STR R1,[R0],#4
	;STR R2,[R0],#4
	SUB R5,#2   ;since you already have fib(1) and fib(2)
	
FIB	ADD R3,R1,R2
	MOV R1,R2
	MOV R2,R3
	STR R1,[R0],#4
	SUB R5,#1
	CMP R5,#0
	BNE FIB
	
	STR R2,[R0]
STOP
	B STOP
COUNT DCD 8
	AREA MYDATA, DATA, READWRITE
DEST DCD 0
	END
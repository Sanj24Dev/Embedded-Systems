; packed bcd to ascii conversion
; 0-9:30-39 
; 6789:36 37 38 39
	AREA RESET,CODE,READONLY
	EXPORT __Vectors
__Vectors
	DCD 0X10001000
	DCD Reset_Handler
	ALIGN
	AREA mycode,CODE,READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=SRC
	MOV R1,#4; COUNTER
	MOV R2,#0
	LDR R5,=RESULT
up	LDRB R3,[R0],#1
	AND R4,R3,#0X0F
	ADD R4,#0x30
	STRB R4,[R5],#1
	AND R4,R3,#0XF0
	LSR R4,#4
	ADD R4,#0x30
	STRB R4,[R5],#1
	SUBS R1,#1
	BNE up
STOP
	B STOP
	
SRC DCD 0X12345678
	AREA MYDATA,DATA,READWRITE
RESULT DCD 0
	END
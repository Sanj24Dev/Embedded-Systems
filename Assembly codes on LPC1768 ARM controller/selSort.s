	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x40001000 ; stack pointer value when stack is empty
	DCD Reset_Handler ; reset vector
	ALIGN
	AREA MYCODE, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=DST
	LDR R1,=ARR
	MOV R2,#10
	
UP  LDR R3,[R1],#4
    STR R3,[R0],#4
    SUBS R2,R2,#1		;decrement counter
	BNE UP
	LDR R0,=DST
oLOOP	
	MOV R1,R0
	MOV R3,R0
	ADD R3,#4
	ADD R10,R9,#1
	
iLOOP
	LDR R4,[R3]
	LDR R7,[R1]
	CMP R4,R7
	BCS UP2
	MOV R1,R3
	
UP2	ADD R3,#4
	ADD R10, #1
	CMP R10,#10
	BNE iLOOP
	LDR R2,[R0]
	LDR R6,[R1]
	STR R2,[R1]
	STR R6,[R0]
	ADD R0,#4
	ADD R9,#1
	CMP R9,#9
	BNE oLOOP
STOP 
	B STOP
ARR DCD 0x10,0x05,0x33,0x24,0x56,0x77,0x21,0x04,0x87,0x01
	AREA MYDATA, DATA, READWRITE
DST DCD 0
	END
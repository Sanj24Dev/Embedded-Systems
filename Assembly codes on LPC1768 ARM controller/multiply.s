	AREA RESET, CODE, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA myCode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=N1
	LDR R0,[R0]
	LDR R1,=N2
	LDR R1,[R1]
	UMULL R3,R4,R0,R1
	LDR R0,=RES
	STR R4,[R0],#4
	STR R3,[R0]
STOP 
	B STOP
	
N1 DCD 0X3
N2 DCD 0X3

	AREA MYDATA, DATA, READWRITE
RES DCD 0X00

	END
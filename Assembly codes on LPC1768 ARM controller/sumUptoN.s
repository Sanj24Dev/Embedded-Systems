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
	LDR R0,=N
	LDR R0,[R0]
	MLA R1,R0,R0,R0
	LSR R1,1
	LDR R0,=RES
	STR R1,[R0]
	
STOP
	B STOP
N DCD 8
	AREA MYDATA, DATA, READWRITE
RES DCD 0
	END
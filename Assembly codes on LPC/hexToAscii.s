	AREA RESET, CODE, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA myCode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=NUM
	LDR R0,[R0]
	LDR R1,=RES
	MOV R2,#0XF
	
DIG	AND R4,R0,R2
	CMP R4,#9
	BLS DOWN
	ADD R4,#0X7
DOWN
	ADD R4,#0X30
	STR R4,[R1],#4
	LSR R0,#4
	CMP R0,#0
	BHI DIG
	
STOP 
	B STOP
	
NUM DCD 0X45
	AREA MYDATA, DATA, READWRITE
RES DCD 0
	END
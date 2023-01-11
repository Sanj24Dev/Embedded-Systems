	AREA RESET, CODE, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA myCode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R1,=ARR		;FIRST INDEX
	ldr r9,=num
	ADD R2, R1, #36	;LAST INDEX
	LDR R6,=0
	
UP	LDR R3,[R1]
	LDR R4,[R2]
	STR R4,[R1],#4
	STR R3,[R2],#-4
	ADD R6,#1
	CMP R6,#5
	BNE UP 
	
STOP 
	B STOP
	
num dcd 0x11112222,0x45679876,0x4567abfd,0xadef4321
	AREA MYDATA, DATA, READWRITE
ARR DCD 1,2,3,4,5,6,7,8,9,10
	END
;**********************
; Universidad del Valle de Guatemala
; IE2023: Programación de Microcontroladores
; Lab01.asm
; Autor: Santiago Burgos
; Proyecto: Laboratorio 01
; Hardware: ATMega328P
; Creado: 30/01/2024
;*********************
; ENCABEZADO
;*********************
.include "M328PDEF.inc"
.CSEG
.ORG 0x00 
;*********************
; STACK POINTER 
;*********************

LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17

Setup:
	
	LDI	R20, (1 << CLKPCE)				;CONFIGURAR EL PRESCALER
	STS CLKPR, R20
	LDI R20, 0b0000_0100
	STS CLKPR, R20

	LDI R16, 0b0011_1100			;SALIDA PARA PB
	OUT DDRB, R16
	LDI R17, 0						;almacena el contador1

	LDI R16, 0b0000_1111
	OUT DDRC, R16
	LDI R20, 0						;ALMACENA EL CONTADOR 2

	LDI R16, 0b1111_1011
	OUT DDRD, R16
	LDI R21, 0


Loop:
	SBIC PINB, PB1
	CALL AUMENTAR
	SBIC PINB, PB0
	CALL DECREMENTAR			;CONTADOR 1

	SBIC PINC, PC5				
	CALL AUMENTAR2
	SBIC PINC, PC4
	CALL DECREMENTAR2			;CONTADOR 2

	SBIC PIND, PD2
	CALL SUMADOR				;SUMADOR
	
	 RJMP Loop


;**************************
;subrutinas
;**************************
AUMENTAR:
	CALL DELAY
	LSR R17						;mueve el bit a la derecha
	LSR R17
	INC R17
	LSL R17						;mueve el bit a la izquierda
	LSL R17
	OUT PORTB, R17
	RJMP Loop

DECREMENTAR:
	CALL DELAY
	LSR R17
	LSR R17
	DEC R17
	LSL R17
	LSL R17
	OUT PORTB, R17
	RJMP Loop

AUMENTAR2:
	CALL DELAY
	INC R20
	OUT PORTC, R20
	RJMP Loop

DECREMENTAR2:
	CALL DELAY
	DEC R20
	OUT PORTC, R20
	RJMP Loop



	DELAY:
	LDI R18, 0
	LDI R19, 0
	BUCLE1:
	INC R18
	CPI R18, 0
	BRNE BUCLE1
	INC R19
	CPI R19,0
	BRNE BUCLE1
	RET

SUMADOR:
	CALL DELAY
	MOV R21, R17
	ADD R21, R20
	LSL R21
	LSL R21
	LSL R21
	OUT PORTD, R21
	RJMP Loop


https://youtu.be/lH_pM_9p05g				; Link de YouTube para ver la explicación del video



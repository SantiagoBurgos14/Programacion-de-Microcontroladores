;**********************
; Universidad del Valle de Guatemala 
; IE2023:: Programación de Microcontroladores
; Lab02.asm
; Autor: Santiago Burgos
; Proyecto: Laboratorio 2
; Hardware: ATMega328P
; Creado: 20/02/2024
;**********************
; ENCABEZADO
;************************
.include "M328PDEF.inc"
.cseg
.org 0x00
//*************************
// Stack
LDI R16, LOW(RAMEND)
OUT SPL, R16
LDI R17, HIGH(RAMEND)
OUT SPH, R17
//*************************
// CONFIGURACION
//*************************

Setup:
	
	CALL Init_T0
	ldi R17, 0b0001_1110
	out DDRB, R17
	LDI R20, 0							;CONTADOR DE 10 MS
	ldi R19, 0							;CONTADOR
	ldi R21, 15


LOOP:
	IN R16, TIFR0
	SBRS R16, OCF0A
	RJMP LOOP
	
	SBI TIFR0, OCF0A					;APAGA

	INC R20
	CPI R20, 100						;CUENTA HASTA LOS 100MS
	BRNE LOOP
	CLR R20								;SE BORRA EL CONTADOR
	LSR R19

	PI R19, 15
	BREQ leds_cero

	INC R19

	LSL R19
	OUT PORTB, R19		; muestra en binario
	RJMP LOOP



 leds_cero:
	CLR R19
	OUT PORTB, R19

	

//********
Init_T0: 

	LDI R16, 0
	OUT TCNT0, R16

	LDI R16, 156
	OUT OCR0A, R16

	LDI R16, (1 << WGM01) ; 0b0000_0010 
	OUT TCCR0A, R16

	LDI R16, (1 << CS02)|(1 << CS00)
	OUT TCCR0B, R16

RET

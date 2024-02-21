;**********************
; Universidad del Valle de Guatemala 
; IE2023:: Programación de Microcontroladores
; PreLab3.asm
; Autor: Santiago Burgos
; Proyecto: Laboratorio 3
; Hardware: ATMega328P
; Creado: 06/02/2024
;**********************
; ENCABEZADO
;************************
.include "M328PDEF.inc" 
.cseg 
.org 0x0000 
JMP MAIN ; vector reset 
.org 0x0006 ; vector de ISR: PCINT0 

JMP ISR_PCINT0 

MAIN: 
LDI R16, LOW(RAMEND) 
OUT SPL, R16 
LDI R17, HIGH(RAMEND) 
OUT SPH, R17  

SETUP: 

SBI DDRB, PB5					 ;SALIDA
CBI PORTB, PB5					
LDI R16, (1 << PC3)|(1 << PC2)|(1 << PC1)|(1 << PC0) 
OUT DDRC, R16					 ; DECLARA SALIDAS

LDI R16, 0 
OUT PORTC, R16 

SBI PORTB, PB0					 ;PULLUP
CBI DDRB, PB0					; BOTON 1

SBI PORTB, PB1					; pullup 
CBI DDRB, PB1					 ;BOTON 2

LDI R16, (1 << PCINT1)|(1 << PCINT0) 
STS PCMSK0, R16						 ; HAB PCINT en PCINT0 y PCINT1 
LDI R16, (1 << PCIE0) 
STS PCICR, R16						 ; HABILITA ISR PCINT[7:0] PARA TENER UN CONTROL DE REGISTROS 
SEI									; HABILITA INTERRUPCIONES 

LDI R20, 0 


LOOP: 
OUT PORTC, R20
JMP LOOP 

ISR_PCINT0: 

PUSH R16							;GUARDA EN LA PILA
IN R16, SREG 
PUSH R16 
IN R18, PINB 
SBRC R18, PB0 
JMP CHECKPB1 
INC R20 
CPI R20, 16 
BRNE SALIR 
LDI R20, 0 
JMP SALIR 

CHECKPB1: 
SBRC R18, PB1 
JMP SALIR 
DEC R20 
BRNE SALIR 
LDI R20, 15 

 
SALIR: 
SBI PINB, PB5							; TOGGLE
SBI PCIFR, PCIF0						; BANDERA ISR PCINT0 OFF 
POP R16									; obtener valores 
OUT SREG, R16 
POP R16 
RETI  
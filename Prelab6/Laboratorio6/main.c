/*
 * Laboratorio6.c
 *
 * Created: 23/04/2024 1:43:46 p. m.
 * Author : SANTIAGO
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void initUART9600(void);  
void wUART(char singleChar);


volatile char bufferRX;		//Volatil porque cambia 

int main(void)
{
	initUART9600();
	sei();
	DDRB = 0xFF; // Salidas
	DDRD = 0XFF;
	
	wUART('H');
	wUART('A');
	wUART('L');
	wUART('L');
	wUART('O');
	PORTB=0;
	PORTD=0;
	
    while (1) 
    {
		
		
    }
}

void initUART9600(void){
	//Configurar RXy TX
	DDRD &= ~(1 << DDD0);		//Rx entrada
	DDRD |= (1 << DDD1);		//TX salida
	
	
	UCSR0A = 0;
	UCSR0A |= (1 << U2X0); // Fast Mode de comunicacion 
	
	
	UCSR0B = 0;			//configuracion de registros
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); //activa interrupcion para rx y TX  
	
	
	UCSR0C = 0;			// configuracion de registros
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);		// tamaño del caracter un bit y sin pariedad
	
	UBRR0 = 207;		// 9600 Baud rate
}


void wUART(char singleChar){
	
	while(!(UCSR0A & (1 << UDRE0)));  // si el bufer esta lleno no envia ni recibe
	UDR0 = singleChar;  //Lee y guarda
	
}

//ISR, recieve
ISR(USART_RX_vect){
	bufferRX = UDR0;

	while(!(UCSR0A & (1 << UDRE0)));   // si el bufer esta lleno no envia ni recibe
	UDR0 = bufferRX;  // muestra lo guardado en buffer
	
	PORTB = bufferRX;   //Convertir un caracter a entero
}

//buffer es el canal de comunicacion 
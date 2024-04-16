/*
 * Universidad del Valle de Guatemala
 * Laboratorio4.c
 *
 * Created: 4/8/2024 11:20:05 PM
 * Author : Santiago Burgos
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t counter = 0;
uint8_t display[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

/*
*******
*/

void initPCINT1(void);

void initADC(void);

/*
*******
*/

int main(void){

	cli();
	DDRD = 0xFF;			//PORTD COMO SALIDA
	PORTD = 0;					
	UCSR0B = 0;				// DESACTIVO RX Y TX
	
	DDRB |= (1 << PINB0) | (1 << PINB1) | (1 << PINB2) | (1 << PINB4);
	PORTB = 0;
	PORTB |= (1 << PORTB1);

	PORTC|= (1 << PINC0) | (1 << PINC1);
	DDRC &= ~(1 << PINC0) | ~(1 << PINC1);			//PUSHBUTTONS

	
	initPCINT1();
	initADC();	
	
	sei();



while (1){

	ADCSRA |= (1<<ADSC);
	//_delay_ms(5);

	uint8_t valorHex = ADCH;
	uint8_t valor1 = valorHex/16;
	uint8_t valor2 = valorHex%16;

	// Multiplexacion	
	
	
	//PORTD |= (1<<PD0);
	PORTD = display[valor1];
	PORTB |= (1 << PB0);
	PORTB &= ~((1 << PB1) | (1 << PB2));
	
	//PORTD = valorHex;
	_delay_ms(5);
	
	//PORTB = 0;
	PORTD = display[valor2];
	PORTB |= (1 << PB1);
	PORTB &= ~((1 << PB0) | (1 << PB2));
	
	_delay_ms(5);
	
	
	//PORTB = 0;
	PORTD = counter;
	PORTB |= (1 << PB2);
	PORTB &= ~((1 << PB0) | (1 << PB1));
	
	_delay_ms(5);
	
	if (valorHex > counter){
		PORTB |= (1 << PORTB4);
		} else {
		PORTB &= ~(1 << PORTB4);
	}

}

}

void initPCINT1(){

	PCICR |= (1 << PCIE1);	
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);	

}


void initADC(void){
	
	ADMUX |= 0x02;
	
	//Vref = AVcc = 5Vs<
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1 << REFS1);
	
	// justifica hacia la izquierda
	ADMUX |= (1 << ADLAR);
	
	ADCSRA = 0;
	
	// encendiendo ADC
	ADCSRA |= (1<<ADEN);
	
	// habilita isr (interrupcion ) ADC
	
	ADCSRA |= (1<<ADIE);
	
	// prescaler de 128 > 16M / 128 = 125khz
	
	ADCSRA |= (1<<ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// deshabilitar la entrada digital de PC2????
	DIDR0|= (1 << ADC2D);
	 
	
}

ISR (ADC_vect){
	
	ADCSRA |= (1<<ADIF);
}

ISR(PCINT1_vect){
	
	if (!(PINC & (1 <<PINC0))){
		counter++;
		} else if(!(PINC &(1 <<PINC1))){
		counter--;
		}else{
		counter = counter;
	}
	
}
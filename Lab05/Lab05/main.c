/*
 * Laboratorio5.c
 *
 * Created: 16/04/2024 2:04:43 p. m.
 * Author : SANTIAGO
 */ 


#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "PWM1/PWM1.h"  //son comillas porque es un alibreria local 

uint8_t Seleccion = 0;
uint16_t a = 0;
uint16_t b = 1000;
uint16_t c = 0;
uint16_t d = 1000;

void initADC(void);

int main(void)
{
	
	cli();
	DDRB |= (1<<PORTB2)|(1<<PORTB1);	//Canales de salida
	DDRD |= (1<<PORTD5);
	
	PORTD = 0;
		
	DDRC = 0;
	
	TCCR1A |= (1 << COM1B1)|(1<< COM1A1);  // no invertido (1ms en alto y 19 en bajo)
	//TCCR1A |= (1 << WGM11);			// PWM con ICR (valMax)
	//TCCR1B |= (1 << WGM12) | (1 << WGM13);
	initFastPWM1();
	//TCCR1B |= (1 << CS11);//Prescaler 8
	Prescaler(8);
	
	
	
	ICR1 = 39999;    //valMax
	
	a = 0;
	b = 1000;
	OCR1B = b;
	
	c = 0;
	d = 1000;
	OCR1A = d;
	
	//////////////////////////////////////////////////////////timer 0
	DDRD |= (1 << PORTD6) | (1 << PORTD5);		
	TCCR0A = 0;
	TCCR0B = 0;
	
	//no inverted and fast PWM, TOP 255 (default)
	TCCR0A |= (1 << COM0A1) | (1  << WGM00) | (1 << WGM01) | (1 << COM0B1) | (1 << COM0B0);
	
	//Prescaler 1024
	TCCR0B = 0x05;

	initADC();
	
	sei();
	
	while (1)
	{
		ADCSRA |= (1 << ADSC); // ADSC inicia la conversión
		_delay_ms(2);
		
		
		
		
	}
}

void initADC(void){
	
	ADMUX = 0;
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
	// deshabilitar la entrada digital
	DIDR0|= (1 << ADC0D)|(1<<ADC1D)|(1<< ADC2D);
	
}

ISR (ADC_vect){				// interrupcion
	
	ADCSRA |= (1<<ADIF);
	
	if (Seleccion == 0){
		//ADMUX |= 0x00;
		//ADMUX &= ~(0x01);
		ADMUX = (ADMUX & 0xF0);
		//a = ADCH; 
		OCR1B = (200/12)*ADCH+1000;
		Seleccion = 1;
		
	} else if (Seleccion == 1){
		//ADMUX |= 0x01;
		//ADMUX &= ~(0x00);
		ADMUX = (ADMUX & 0xF0) | 1;
		//c = ADCH;
		//d = (200/12)*c+1000;
		OCR1A = (200/12)*ADCH+1000;
		Seleccion = 2;
		
	}else if (Seleccion == 2){
			//ADMUX |= 0x02;
			//ADMUX &= ~(0x01);
			ADMUX = (ADMUX & 0xF0) | 2;
			OCR0B = ADCH;
			Seleccion = 0; //siguiente seleccion
		
	}
	
	
}
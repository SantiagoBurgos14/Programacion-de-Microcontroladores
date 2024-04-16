/*
 * GccApplication1.c
 *
 * Created: 15/04/2024 5:19:46 p. m.
 * Author : SANTIAGO
 */ 

#include <avr/io.h>


int main(void)
{
    DDRD |= (1 << PORTD6) | (1 << PORTD3);		//PB6 as output (OC0A and OCR0B)
    TCCR0A = 0;
    TCCR0B = 0;
    
    //no inverted and fast PWM, TOP 255 (default)
    TCCR0A |= (1 << COM0A1) | (1  << WGM00) | (1 << WGM01) | (1 << COM0B1) | (1 << COM0B0);
    
    //Prescaler 1024
    TCCR0B = 0x05;

    while (1) 
    {
		for (uint8_t i = 0; i < 255; i++){
			OCR0A = i;
			OCR0B = i;
			_delay_ms(20);
    }
	}


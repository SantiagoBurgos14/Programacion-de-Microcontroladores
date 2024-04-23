/*
 * PWM1.c
 *
 * Created: 4/21/2024 12:34:03 AM
 *  Author: Pato Burgerstein
 */ 

#include "PWM1.h"


// ADCH = lee el potenciometro, H es porque lee la parte alta de ese reigstro

/*
ADMUX == 

si se trabjaa onterrupciones en ADC
ya esta deicido el canal en la interrupcion 
no se peude decidir

*/

void initFastPWM1(void){
	TCCR1A |= (1 << WGM11);			// PWM con ICR (valMax)
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
}

void Prescaler(uint8_t prescaler){
	if (prescaler == 8){
		TCCR1B |= (1 << CS11);//Prescaler 8
	}
	else if (prescaler == 1024){
		TCCR1B |= (1 << CS12) | (1 << CS10);//Prescaler 12
	}
}

void ConfCanalNinverted(void){
	TCCR1A |= (1 << COM1B1)|(1<< COM1A1);
}

void Servo1(void){
	OCR1B = (200/12)*ADCH+1000;
}

void Servo2(void){
	OCR1A = (200/12)*ADCH+1000;
}


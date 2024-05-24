/*
 * Proyecto2.c
 *
 * Created: 6/05/2024 5:49:49 p. m.
 * Author : SANTIAGO
 */ 



// PC0 - PC3 potenciometros posbiblemente joysticks
// PB5 y PB4 Pulsadores para cambiar de modo
// PD2,3,4 led RGB que va a indicar el modo
// Timer 1 
// Timer 0


#define F_CPU 16000000

////////////////////////////////////////////////////////////////////////////
//LIBRERIAS
////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/eeprom.h> 
#include <stdio.h>
#include "Servos/Servos.h"
#include "MOTORES/MOTORES.h"

////////////////////////////////////////////////////////////////////////////
// Variables Globales
////////////////////////////////////////////////////////////////////////////

float adcValue1 = 0;
float adcValue2 = 0;
float adcValue3 = 0;
float adcValue4 = 0;
float dutyCycle = 0;
float dutyCycle2 = 0;

volatile uint8_t Estado = 0;

volatile char bufferRx;

char buffer[10];        // envio de dato

int LecturaEEPROM = 0;


int EstadoGuardarEEPROM = 0;
int EstadoLeerEEPROM = 0;

int contadorGuardar = 0;

////////////////////////////////////////////////////////////////////////////
//DEFINICIONES
////////////////////////////////////////////////////////////////////////////

#define LEDAZUL PB3
#define LEDVERDE PB4
#define LEDROJO PB5
#define AVANZAR  PC4
#define GRABAR PC5

////////////////////////////////////////////////////////////////////////////
// FUNCIONES
////////////////////////////////////////////////////////////////////////////

void ADC_init(void);
uint16_t adcRead(uint8_t);

void initUART(void);
void writeUART(char caracter);
void writeTextUART(char* texto);


void pruebaServos(void);

////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////

int main (void){
	
	cli();
	
	DDRD = 0xFF;	// Salidas
	
	PWM_init();
	PWM0_init();
	ADC_init();
	
	initUART();
	
	
	
	sei();
	
	
	
	while (1){
		
		switch (Estado)
		{
// caso manual ///////////////////////////////////
		case 0:	// Estado Manual
		
		PORTB |= (1 << LEDVERDE);
		PORTB &= ~(1<< LEDAZUL);
		PORTB &= ~(1<< LEDROJO);
		
		pruebaServos();

				
			break;
			
// Estado para guardar movimientos	/////////////////////////////////////////

		case 1: 
				
		PORTB |= (1 << LEDROJO) | (1 << LEDAZUL);
		PORTB &= ~(1 << LEDVERDE);
		
		pruebaServos();
		
		
		if (EstadoGuardarEEPROM == 1)			// Si estadoGuardarEEPROM = 1 permite guardad, si =0 no guarda
		{
			
			adcValue1 = map(adcValue1, 0, 1023, 0, 255);
			int EepromTemporal = 0 + contadorGuardar;
			eeprom_write_byte((uint8_t*)EepromTemporal, adcValue1);
			_delay_ms(5);
			
			EepromTemporal = 1 + contadorGuardar;
			adcValue2 = map(adcValue2, 0, 1023, 0, 255);
			eeprom_write_byte((uint8_t*)EepromTemporal, adcValue2);
			_delay_ms(5);			
			
			EepromTemporal = 2 + contadorGuardar;
			adcValue3 = map(adcValue3, 0 , 1023, 0, 75);
			eeprom_write_byte((uint8_t*)EepromTemporal, adcValue3);
			_delay_ms(5);
			
			EepromTemporal = 3 + contadorGuardar;
			adcValue4 = map(adcValue4, 0, 1023, 0, 50);
			eeprom_write_byte((uint8_t*)EepromTemporal, adcValue4);
						
			EstadoGuardarEEPROM = 0;
		}
		 
		
		
		
		
		
			break;
			
// Estado para leer los movimientos
		case 2:	
			
		PORTB |= (1 << LEDVERDE) | (1 << LEDAZUL);
		PORTB &= ~(1 << LEDROJO);
		
		if (EstadoLeerEEPROM == 1){			// Si EsstadoLeerEEPROM = 1 activa los movimientos
			int lecturaEEPROMTemporal0 = 0 + contadorGuardar;
			LecturaEEPROM = eeprom_read_byte((uint8_t*)lecturaEEPROMTemporal0);
			LecturaEEPROM = map (LecturaEEPROM, 0, 255, 0, 1023);
			servo_writeA(LecturaEEPROM);
			_delay_ms(5);
			
			int lecturaEEPROMTemporal1 = 1 + contadorGuardar;
			LecturaEEPROM = eeprom_read_byte((uint8_t*)lecturaEEPROMTemporal1);
			LecturaEEPROM = map (LecturaEEPROM, 0, 255, 0, 1023);
			servo_writeB(LecturaEEPROM);
			_delay_ms(5);
			
			int lecturaEEPROMTemporal2 = 2 + contadorGuardar;
			LecturaEEPROM = eeprom_read_byte((uint8_t*)lecturaEEPROMTemporal2);
			LecturaEEPROM = map (LecturaEEPROM, 0, 255, 0, 75);
			PWM0_dca(LecturaEEPROM, NO_INVERTING);
			_delay_ms(5);
			
			int lecturaEEPROMTemporal3 = 0 + contadorGuardar;
			LecturaEEPROM = eeprom_read_byte((uint8_t*)lecturaEEPROMTemporal3);
			LecturaEEPROM = map (LecturaEEPROM, 0, 255, 0, 50);
			PWM0_dcb(LecturaEEPROM, NO_INVERTING);
			_delay_ms(5);
			
			EstadoLeerEEPROM = 0;
		}
		
		
		
		
		// int a caracter
		sprintf(buffer, "%d", LecturaEEPROM);
		// enviar valor
		writeTextUART("Valor de variable: ");
		writeTextUART(buffer);
		writeTextUART("\n");
		_delay_ms(1000);
			break;
		
		case 3:	// Estado para UART
		
		PORTB |= (1 << LEDROJO);
		PORTB &= ~(1<< LEDAZUL);
		PORTB &= ~(1<< LEDVERDE);
		
		break;
		
		}
		
		
	}
	
	
}

void ADC_init(void){
	ADMUX |= (1<<REFS0);	// VCC REF
	ADMUX &= ~(1<<REFS1);
	ADMUX &= ~(1<<ADLAR);	// 10 bits
	// PRESCALER 128 > 16M/128 = 125KHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRA |= (1<<ADEN);	// ADC ON
	
	DDRC &= ~(1 << PINC4);		//BOtones
	PORTC |= (1 << PORTC4) | (1<< PORTC5);		// Pull Up
	
	DDRC &= ~(1 << PINC5);
	
	PCICR |= (1 << PCIE1);  // Habilitar interrupciones de pin change para PCINT[14:8] (puerto C)
	PCMSK1 |= (1 << PCINT12) | (1 << PCINT13);  // Habilitar interrupción por cambio de pin en PC4 (PCINT12)
}


uint16_t adcRead(uint8_t canal){
	ADMUX = (ADMUX & 0xF0)|canal;	// selección de canal
	ADCSRA |= (1<<ADSC);	// inicia conversión
	while((ADCSRA)&(1<<ADSC));	// hasta finalizar conversión
	return(ADC);
}


ISR(PCINT1_vect){
	//#define AVANZAR PC4
	//#define GRABAR PC5
	
	_delay_ms(10);	//Antirebote
	if (!(PINC & (1 << AVANZAR)))
	{
		Estado = Estado +1;
		if (Estado >= 4)
		{
		Estado = 0;	
		}
		
	} 
	
	if (!(PINC & (1 << GRABAR)))
	{
		if (Estado == 1){			// pulsador guarda			
		 if (PINC & ( 1 << GRABAR))
		 {
			 contadorGuardar = contadorGuardar + 10;
			  EstadoGuardarEEPROM = 1;   // Guarda los movimientos en el estado 1
		 }
		
		}
		else if (Estado == 2)		// Reproducir
		{
			if (PINC & ( 1 << GRABAR))
			contadorGuardar = contadorGuardar + 10;
			EstadoLeerEEPROM = 1;		// lee loss movimientos guardados en el estado 1 y los reproduce en el estado 2
		}
	}
}
// adc 10 bit = 1024
// eeprom solo guarda de 0 a 255


void initUART(void){
	// Rx y Tx
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	// fast mode
	// CONFIGURAR A
	UCSR0A = 0;
	UCSR0A |= (1<<U2X0);
	// CONFIGURAR B /  ISR RX/ habilitar Rx y Tx
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	// frame 8 bits, no paridad, 1 bit stop
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	// baudrate 9600
	UBRR0 = 207;
}


void writeUART(char caracter){
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0 = caracter;
}

void writeTextUART(char* texto){
	uint8_t i;
	for (i=0; texto[i] != '\0'; i++){
		while(!(UCSR0A&(1<<UDRE0)));
		UDR0 = texto[i];
	}
}


void pruebaServos(void){
		// Servomotores
		
		adcValue1 = adcRead(0);
		servo_writeA(adcValue1);
		_delay_ms(10);
		adcValue2 = adcRead(1);
		servo_writeB(adcValue2);
		_delay_ms(10);
		
		// Motores DC			
				
		adcValue3 = adcRead(2);
		adcValue4 = adcRead(3);
		dutyCycle = map(adcValue3, 512, 1023, 0, 75);
		dutyCycle2 = map(adcValue4, 500, 1023, 0, 50);
		if (adcValue3 >= 520)
		{
			
			PWM0_dca(dutyCycle, NO_INVERTING);
			//PORTD |= (1 << PD3);
			//PORTD &= ~( 1<< PD2);
			
			PWM0_dcb(0, NO_INVERTING);
			//PORTD |= (1 << PD4);
			//PORTD &= ~( 1<< PD7);
			
			}
			else if (adcValue4 <= 500)
			{
				//PORTD |= (1 << PD2);
				//PORTD &= ~(1 << PD3);
				//PORTD |= (1 << PD7);
				//PORTD &= ~(1 << PD4);
				
				
				PWM0_dca(0, NO_INVERTING);
				PWM0_dcb(dutyCycle2, NO_INVERTING);
				
			}
			
			else {
			PWM0_dca(0, NO_INVERTING);
			PWM0_dcb(0, NO_INVERTING);
		}
		
		
			
		
	
}



//init uar, wirte uart y no seque mas uartS

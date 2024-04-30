/*
 * Laboratorio6T.c
 *
 * Created: 23/04/2024 3:12:23 p. m.
 * Author : SANTIAGO
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void initUART9600(void);
void wUART(char singleChar);
void writeTxt(char text[]); // lista
void initADC(void);
int adcRead(uint8_t canal);
int adcPot=0;

volatile char bufferRX;		//Volatil porque cambia
int State = 0;

int main(void)
{
	initUART9600();
	initADC();
	sei();
	DDRB = 0xFF; // Salidas
	DDRD = 0XFF;
	
	PORTB=255;
	PORTD=255;
	_delay_ms(1000);

	while (1)
	{
		
		switch (State)
		{
			case 0:
			writeTxt("1. Leer el Potenciometro \n");
			writeTxt("2. Enviar ASCII \n");
			_delay_ms(10000);
			
						
			break;
			case 1:
				writeTxt("Valor del potenciometro: \n ");
				adcPot = adcRead(0);
				PORTB = adcPot;
				PORTD = adcPot;
				
				_delay_ms(1000);
				State = 0;
				
				break;
				
			case 2:
				
				writeTxt("Mostrar ASCII\n");
				_delay_ms(1000);
				break;
		}
		
		
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


void initADC(void){
	ADMUX |= (1<<REFS0);	// VCC REF
	ADMUX &= ~(1<<REFS1);
	ADMUX &= ~(1<<ADLAR);	// 10 bits
	// PRESCALER 128 > 16M/128 = 125KHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRA |= (1<<ADEN);	// ADC ON
	
}

int adcRead(uint8_t canal){
	ADMUX = (ADMUX & 0xF0)|canal;	// selección de canal
	ADCSRA |= (1<<ADSC);	// inicia conversión
	while((ADCSRA)&(1<<ADSC));	// hasta finalizar conversión
	return(ADC);
}

void wUART(char singleChar){
	
	while(!(UCSR0A & (1 << UDRE0)));  // si el bufer esta lleno no envia ni recibe
	UDR0 = singleChar;  //Lee y guarda
	
}


void writeTxt(char text[]){
	for (uint8_t a = 0; text[a] != '\0';a++){   // siempre recorre la lista hasta que llega a \0(nulo) cuando no hay ningun caracter se detiene
		while(!(UCSR0A & (1 << UDRE0)));  // Bucle, lee hasta que se cumple 
		UDR0 = text[a];   // recoge el dato y lo almacena en otra variable 
	}
}
//ISR, recieve
ISR(USART_RX_vect){
	bufferRX = UDR0;

	while(!(UCSR0A & (1 << UDRE0)));   // si el bufer esta lleno no envia ni recibe
	UDR0 = bufferRX;  // muestra lo guardado en buffer
	switch(State){
		case 0:
		if (bufferRX == '1'){
			State = 1;
		} else if (bufferRX == '2'){
			State = 2;
		}
		case 2:
			UDR0 = bufferRX;
			PORTB = bufferRX;   //Convertir un caracter a entero
			PORTD = bufferRX;
			if (bufferRX == '0'){
				State = 0;
			}
			
	}
	
	
}




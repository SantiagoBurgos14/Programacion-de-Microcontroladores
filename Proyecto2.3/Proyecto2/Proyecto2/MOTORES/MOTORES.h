/*
 * MOTORES.h
 *
 * Created: 14/05/2024 12:18:19 p. m.
 *  Author: SANTIAGO
 */ 


#ifndef MOTORES_H_
#define MOTORES_H_

#include <avr/io.h>

#define NO_INVERTING	1
#define INVERTING		0


void PWM0_init(void);
void PWM0_dca(uint8_t dc, uint8_t modo);
void PWM0_dcb(uint8_t dc, uint8_t modo);


#endif /* MOTORES_H_ */
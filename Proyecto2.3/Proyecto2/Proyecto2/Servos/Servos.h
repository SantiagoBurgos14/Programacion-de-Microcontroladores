/*
 * IncFile1.h
 *
 * Created: 14/05/2024 12:03:11 p. m.
 *  Author: SANTIAGO
 */ 


#ifndef SERVOS_H_
#define SERVOS_H_


#include <avr/io.h>

void PWM_init(void);
void servo_writeA(float valADC);
void servo_writeB(float valADC);
float map(float x, float in_min, float in_max, float out_min, float out_max);


#endif /* SERVOS_H_ */






/*
 * PWM1.h
 *
 * Created: 4/21/2024 12:29:39 AM
 *  Author: Pato Burgerstein
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <avr/io.h>
#include <stdint.h>

void initFastPWM1(void);
void Prescaler(uint8_t prescaler);
void ConfCanalNinverted(void);
void Servo1(void);
void Servo2(void);


#endif /* PWM1_H_ */
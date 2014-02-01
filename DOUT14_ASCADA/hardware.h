/*
 * hardware_in.h
 *
 *  Created on: 04.09.2013
 *      Author: Администратор
 */

#ifndef HARDWARE_OUT_H_
#define HARDWARE_OUT_H_

#define INPUT1 PINC & (1<<PC0)
#define INPUT2 PINC & (1<<PC1)
#define INPUT3 PINC & (1<<PC2)
#define INPUT4 PINC & (1<<PC3)
#define INPUT5 PINC & (1<<PC4)
#define INPUT6 PINC & (1<<PC5)
#define INPUT7 PIND & (1<<PD2)
#define INPUT8 PIND & (1<<PD3)
#define INPUT9 PIND & (1<<PD6)
#define INPUT10 PIND & (1<<PD7)
#define INPUT11 PINB & (1<<PB0)
#define INPUT12 PINB & (1<<PB1)
#define INPUT13 PINB & (1<<PB2)
#define INPUT14 PINB & (1<<PB3)
#define INPUT15 PINB & (1<<PB4)
#define INPUT16 PINB & (1<<PB5)

#define OUTPUT1_1  PORTC |= (1<<PC0)
#define OUTPUT2_1  PORTC |= (1<<PC1)
#define OUTPUT3_1  PORTC |= (1<<PC2)
#define OUTPUT4_1  PORTC |= (1<<PC3)
#define OUTPUT5_1  PORTC |= (1<<PC4)
#define OUTPUT6_1  PORTC |= (1<<PC5)
#define OUTPUT7_1  PORTD |= (1<<PD2)
#define OUTPUT8_1  PORTD |= (1<<PD3)
#define OUTPUT9_1  PORTD |= (1<<PD6)
#define OUTPUT10_1 PORTD |= (1<<PD7)
#define OUTPUT11_1 PORTB |= (1<<PB0)
#define OUTPUT12_1 PORTB |= (1<<PB1)
#define OUTPUT13_1 PORTB |= (1<<PB2)
#define OUTPUT14_1 PORTB |= (1<<PB3)
#define OUTPUT15_1 PORTB |= (1<<PB4)
#define OUTPUT16_1 PORTB |= (1<<PB5)

#define OUTPUT1_0  PORTC &= ~(1<<PC0)
#define OUTPUT2_0  PORTC &= ~(1<<PC1)
#define OUTPUT3_0  PORTC &= ~(1<<PC2)
#define OUTPUT4_0  PORTC &= ~(1<<PC3)
#define OUTPUT5_0  PORTC &= ~(1<<PC4)
#define OUTPUT6_0  PORTC &= ~(1<<PC5)
#define OUTPUT7_0  PORTD &= ~(1<<PD2)
#define OUTPUT8_0  PORTD &= ~(1<<PD3)
#define OUTPUT9_0  PORTD &= ~(1<<PD6)
#define OUTPUT10_0 PORTD &= ~(1<<PD7)
#define OUTPUT11_0 PORTB &= ~(1<<PB0)
#define OUTPUT12_0 PORTB &= ~(1<<PB1)
#define OUTPUT13_0 PORTB &= ~(1<<PB2)
#define OUTPUT14_0 PORTB &= ~(1<<PB3)
#define OUTPUT15_0 PORTB &= ~(1<<PB4)
#define OUTPUT16_0 PORTB &= ~(1<<PB5)

#endif /* HARDWARE_OUT_H_ */

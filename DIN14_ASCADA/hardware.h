/*
 * hardware_in.h
 *
 *  Created on: 04.09.2013
 *      Author: Администратор
 */

#ifndef HARDWARE_IN_H_
#define HARDWARE_IN_H_

//inputs configuration

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

#endif /* HARDWARE_IN_H_ */

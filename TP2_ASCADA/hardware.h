/*
 * hardware.h
 *
 *  Created on: 07.09.2013
 *      Author: TCom
 */
/*
 * hardware_in.h
 *
 *  Created on: 04.09.2013
 *      Author: Администратор
 */

#ifndef HARDWARE_OUT_H_
#define HARDWARE_OUT_H_

#define OUTPUT1_0  PORTD |= (1<<PD6)
#define OUTPUT2_0  PORTD |= (1<<PD7)
#define OUTPUT3_0  PORTB |= (1<<PB0)
#define OUTPUT4_0  PORTB |= (1<<PB1)

#define OUTPUT5_0  PORTB |= (1<<PB2)
#define OUTPUT6_0  PORTB |= (1<<PB3)
#define OUTPUT7_0  PORTB |= (1<<PB4)
#define OUTPUT8_0  PORTB |= (1<<PB5)


#define OUTPUT1_1  PORTD &= ~(1<<PD6)
#define OUTPUT2_1  PORTD &= ~(1<<PD7)
#define OUTPUT3_1  PORTB &= ~(1<<PB0)
#define OUTPUT4_1  PORTB &= ~(1<<PB1)

#define OUTPUT5_1  PORTB &= ~(1<<PB2)
#define OUTPUT6_1  PORTB &= ~(1<<PB3)
#define OUTPUT7_1  PORTB &= ~(1<<PB4)
#define OUTPUT8_1  PORTB &= ~(1<<PB5)


#endif /* HARDWARE_OUT_H_ */


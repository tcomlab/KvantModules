/*
 * hardware_dt.h
 *
 *  Created on: 04.09.2013
 *      Author: Администратор
 */

#ifndef HARDWARE_DT_H_
#define HARDWARE_DT_H_
/*
1 - С.0
2 - С.1
3 - С.2
4 - С.3
5 - С.4
6 - С.5

7 - D.2
8 - D.3
9 - D.6
10 - D.7

11 - B.0
12 - B.1
13 - B.2
14 - B.3
15 - В.4
16 - В.5
*/
#define DT1 (&PINC,&PORTC,&DDRC,PC0)
#define DT2 (&PINC,&PORTC,&DDRC,PC1)
#define DT3 (&PINC,&PORTC,&DDRC,PC2)
#define DT4 (&PINC,&PORTC,&DDRC,PC3)
#define DT5 (&PINC,&PORTC,&DDRC,PC4)
#define DT6 (&PINC,&PORTC,&DDRC,PC5)

#define DT7 (&PIND,&PORTD,&DDRD,PD2)
#define DT8 (&PIND,&PORTD,&DDRD,PD3)
#define DT9 (&PIND,&PORTD,&DDRD,PD6)
#define DT10 (&PIND,&PORTD,&DDRD,PD7)

#define DT11 (&PINB,&PORTB,&DDRB,PB0)
#define DT12 (&PINB,&PORTB,&DDRB,PB1)
#define DT13 (&PINB,&PORTB,&DDRB,PB2)
#define DT14 (&PINB,&PORTB,&DDRB,PB3)
#define DT15 (&PINB,&PORTB,&DDRB,PB4)
#define DT16 (&PINB,&PORTB,&DDRB,PB5)

#endif /* HARDWARE_DT_H_ */

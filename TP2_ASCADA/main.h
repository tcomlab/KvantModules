/*
 * main.h
 *
 *  Created on: 08.04.2012
 *      Author: TCom
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <intrinsics.h>
#include <ioavr.h>

#define CPU_CLK_Hz 18432000
#define CPU_CLK_kHz (unsigned long)(CPU_CLK_Hz/1000)
#define delay_ns(x) __delay_cycles(x*CPU_CLK_kHz*0.000001)
#define delay_us(x) __delay_cycles(x*(CPU_CLK_Hz/1000000))
#define delay_ms(x) __delay_cycles(x*(CPU_CLK_Hz/1000))
#define delay_s(x)  __delay_cycles(x*CPU_CLK_Hz)

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

#define RX_TX1 PORTD |=  (1<<PD4)
#define RX_TX0 PORTD &= ~(1<<PD4)

#define LED1 PORTD |=  (1<<PD5)
#define LED0 PORTD &= ~(1<<PD5)

//#define ADRESS (((PINB & 0x30)>>4)+0x50)

#endif /* MAIN_H_ */

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
#include <stdbool.h>

#ifndef IAR_COMP                // Эта фишка необходима для правельной работы индексатора еклипса
#define ENABLE_BIT_DEFINITIONS
#include <iom88.h>
#define  __eeprom
#define __flash
#define __no_init
#define __C_task
#endif

#define CPU_CLK_Hz 18432000
#define CPU_CLK_kHz (unsigned long)(CPU_CLK_Hz/1000)
#define delay_ns(x) __delay_cycles(x*CPU_CLK_kHz*0.000001)
#define delay_us(x) __delay_cycles(x*(CPU_CLK_Hz/1000000))
#define delay_ms(x) __delay_cycles(x*(CPU_CLK_Hz/1000))
#define delay_s(x)  __delay_cycles(x*CPU_CLK_Hz)

#define RX_TX1 PORTD |=  (1<<PD4)
#define RX_TX0 PORTD &= ~(1<<PD4)

#define LED1 PORTD |=  (1<<PD5)
#define LED0 PORTD &= ~(1<<PD5)

//#define ADRESS (((PINB & 0x30)>>4)+0x20)

#endif /* MAIN_H_ */

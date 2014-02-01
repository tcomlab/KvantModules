/*
 * SPI.h
 *
 *  Created on: 30.10.2012
 *      Author: TCom
 */

#ifndef SPI_H_
#define SPI_H_

unsigned char spi_transaction(unsigned char);
void spi_init(void);

#define CS_0 PORTB &= ~(1<<PB2)
#define CS_1 PORTB |=  (1<<PB2)

#endif /* SPI_H_ */

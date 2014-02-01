/*
 * SPI.C
 *
 *  Created on: 30.10.2012
 *      Author: TCom
 */

#include "main.h"
#include "SPI.h"

void spi_init(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<PB3)|(1<<PB5)|(1<<PB2);
	DDRB &= ~(1<<PB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	CS_1;
}


unsigned char spi_transaction(unsigned char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

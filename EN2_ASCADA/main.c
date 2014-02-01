/*
 * main.c
 *  ------------------------------------------ DINPUTS ---------------------------------------------------
 *  Created on: 08.04.2012
 *      Author: TCom
 */
#include "LIB_SH/main.h"
#include "LIB_SH/LIB_BLINK/blink.h"
#include "LIB_SH/LIBS_MODBUS/modbus.h"

#define VERSION 10118
//------------------------------------------------------------
volatile long encoder;
void init_hardware(void)
{
	PORTC = 0x00;
	DDRC = 0xFF;

	PORTB = (1<<PB4)|(1<<PB5);
	DDRB = 0xFF;
        DDRB &= ~(1<<PB4)|(1<<PB5);
        
	PORTD = (1<<PD2)|(1<<PD3);
	DDRD =  (1<<PD4)|(1<<PD5);
        EICRA = (1<<ISC01);
        EIMSK = (1<<INT0);
}
//-----------------------------------------------------------
#pragma vector = INT0_vect
__interrupt void encoder1A_interrupt(void)
{
  __enable_interrupt();
    if(!( PIND & (1<<PD3) ))
      encoder++;
    else
      encoder--;
    data_input_register[0] = encoder & 0x0000FFFF;
    data_input_register[1] = (encoder & 0xFFFF0000)>>16;
}
//-----------------------------------------------------------
//-----------------------------------------------------------
void main (void)
{
   encoder =0;
   init_hardware();
   modbus_init();
   blink_init();
   data_holding_register[31]= VERSION;
   __enable_interrupt();
   while(1)
   {
    if (data_holding_register[0] == 0)
    {
      encoder = 0;
      data_holding_register[0] = 1;
      data_input_register[1] = 0;
      data_input_register[0] = 0;
    } 
   }
}

/*
 * main.c
 *  ------------------------------------------ DINPUTS ---------------------------------------------------
 *  Created on: 08.04.2012
 *      Author: TCom
 */
#include "LIB_SH\main.h"
#include "LIB_SH\LIBS_MODBUS\modbus.h"
#include "LIB_SH\LIB_BLINK\blink.h"
#include <string.h>
#include "hardware.h"


#define VERSION 10101
//------------------------------------------------------------
unsigned char togle[16];
//------------------------------------------------------------
void init_hardware(void)
{
	PORTC = 0xFF;
	DDRC = 0x00;

	PORTB = 0xFF;
	DDRB = 0x00;

	PORTD = (1<<PD2)|(1<<PD3)|(1<<PD6)|(1<<PD7);
	DDRD =  (1<<PD4)|(1<<PD5);
}
//-----------------------------------------------------------
void inspect_reset(void)
{
  int i = 0;
  for (i=0;i<14;i++)
  {
	if (data_coil_register[(i>>3)] & (1<<(i&7)))
	{
		data_coil_register[(i>>3)] &= ~(1<<(i&7));
		data_input_register[i] = 0;
	}
  }
}
//-----------------------------------------------------------
void main (void)
{
   init_hardware();
   modbus_init();
   blink_init();
   data_holding_register[31]= VERSION;
   __enable_interrupt();
    while(1)
    {
      inspect_reset();
      delay_ms(1);
      if (INPUT1)  
      {
        data_discret_inputs[0] &= ~(1<<0);
        if (togle[0] == 0){togle[0] = 1;data_input_register[0]++;}
      }
      else 
      {
        data_discret_inputs[0] |= (1<<0); 
        if (togle[0] == 1) togle[0] = 0;
      }
      
      if (INPUT2)  
      {
        data_discret_inputs[0] &= ~(1<<1);
        if (togle[1] == 0){togle[1] = 1;data_input_register[1]++;}
      }                
      else
      {
        data_discret_inputs[0] |= (1<<1); 
         if (togle[1] == 1) togle[1] = 0;
      }
      
      if (INPUT3)  
      {
        if (togle[2] == 0){togle[2] = 1;data_input_register[2]++;}
        data_discret_inputs[0] &= ~(1<<2);
      }
      else
      {
        data_discret_inputs[0] |= (1<<2);
         if (togle[2] == 1) togle[2] = 0;
      }
      
      if (INPUT4)  
      {
        if (togle[3] == 0){togle[3] = 1;data_input_register[3]++;}
        data_discret_inputs[0] &= ~(1<<3);
      }                
      else
      {
        data_discret_inputs[0] |= (1<<3);
         if (togle[3] == 1) togle[3] = 0;
      }
      
      if (INPUT5)
      {
        if (togle[4] == 0){togle[4] = 1;data_input_register[4]++;}
        data_discret_inputs[0] &= ~(1<<4);
      }                
      else 
      {
        data_discret_inputs[0] |= (1<<4);
         if (togle[4] == 1) togle[4] = 0;
      }
      
      if (INPUT6)  
      {
        if (togle[5] == 0){togle[5] = 1;data_input_register[5]++;}
        data_discret_inputs[0] &= ~(1<<5); 
      }                
      else
      {
         if (togle[0] == 1) togle[5] = 0;
        data_discret_inputs[0] |= (1<<5); 
      }
      
      if (INPUT7)
      {
        if (togle[6] == 0){togle[6] = 1;data_input_register[6]++;}
        data_discret_inputs[0] &= ~(1<<6);
      }                
      else 
      {
        if (togle[6] == 1) togle[6] = 0;
        data_discret_inputs[0] |= (1<<6);
      }
      
      if (INPUT8)  
      {
        if (togle[7] == 0){togle[7] = 1;data_input_register[7]++;}
        data_discret_inputs[0] &= ~(1<<7);
      }                
      else
      {
        data_discret_inputs[0] |= (1<<7); 
         if (togle[7] == 1) togle[7] = 0;
      }
      
      if (INPUT9)  
      {
        if (togle[8] == 0){togle[8] = 1;data_input_register[8]++;}
        data_discret_inputs[1] &= ~(1<<0);
      }
      else 
      {
        data_discret_inputs[1] |= (1<<0);
        if (togle[8] == 1) togle[8] = 0;
      }
      
      if (INPUT10) 
      {
        if (togle[9] == 0){togle[9] = 1;data_input_register[9]++;}
        data_discret_inputs[1] &= ~(1<<1);
      }
      else
      {
        data_discret_inputs[1] |= (1<<1);
         if (togle[9] == 1) togle[9] = 0;
      }
      
      if (INPUT11) 
      {
        if (togle[10] == 0){togle[10] = 1;data_input_register[11]++;}
        data_discret_inputs[1] &= ~(1<<2); 
      }
      else 
      {
         if (togle[10] == 1) togle[10] = 0;
        data_discret_inputs[1] |= (1<<2);
      }
      
      if (INPUT12) 
      {
        if (togle[11] == 0){togle[11] = 1;data_input_register[12]++;}
        data_discret_inputs[1] &= ~(1<<3);
      }
      else 
      {
         if (togle[11] == 1) togle[11] = 0;
        data_discret_inputs[1] |= (1<<3);
      }
      
      if (INPUT13) 
      {
        if (togle[12] == 0){togle[12] = 1;data_input_register[12]++;}
         data_discret_inputs[1] &= ~(1<<4);
      }
      else 
      {
         if (togle[12] == 1) togle[12] = 0;
         data_discret_inputs[1] |= (1<<4);
      }
      
      if (INPUT14) 
      {
        if (togle[13] == 0){togle[13] = 1;data_input_register[13]++;}
         data_discret_inputs[1] &= ~(1<<5);
      }
      else                 
      {
         if (togle[13] == 1) togle[13] = 0;
         data_discret_inputs[1] |= (1<<5);
      }
      
      
      if (INPUT15) 
      {
        if (togle[14] == 0){togle[14] = 1;data_input_register[14]++;}
         data_discret_inputs[1] &= ~(1<<6);
      }
      else 
      {
         if (togle[14] == 1) togle[14] = 0;
         data_discret_inputs[1] |= (1<<6);
      }
      
      if (INPUT16) 
      {
        if (togle[15] == 0){togle[15] = 1;data_input_register[15]++;}
         data_discret_inputs[1] &= ~(1<<7);
      }
      else                 
      {
         if (togle[15] == 1) togle[15] = 0;
         data_discret_inputs[1] |= (1<<7);
      }
    }
}

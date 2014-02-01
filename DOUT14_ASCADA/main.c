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

#define VERSION 10102
//------------------------------------------------------------------------------
//extern unsigned int start_state;
extern __no_init unsigned int __eeprom eeprom_register[64];
//------------------------------------------------------------------------------
void init_hardware(void)
{
  //DDRC = 0xFF;
  
  PORTB = (1<<PB4)|(1<<PB5);
  DDRB =  (1<<PB0)|(1<<PB1);
  
  DDRD =  (1<<PD4)|(1<<PD5)|(1<<PD2)|(1<<PD3)|(1<<PD6)|(1<<PD7);
  
  if(eeprom_register[2])  PORTC &= ~(1<<PC0); else PORTC |= (1<<PC0);
  if(eeprom_register[3])  PORTC &= ~(1<<PC1); else PORTC |= (1<<PC1);
  if(eeprom_register[4])  PORTC &= ~(1<<PC2); else PORTC |= (1<<PC2);
  if(eeprom_register[5])  PORTC &= ~(1<<PC3); else PORTC |= (1<<PC3);
  if(eeprom_register[6])  PORTC &= ~(1<<PC4); else PORTC |= (1<<PC4);
  if(eeprom_register[7])  PORTC &= ~(1<<PC5); else PORTC |= (1<<PC5);
  if(eeprom_register[8])  PORTD &= ~(1<<PD2); else PORTD |= (1<<PD2);
  if(eeprom_register[9])  PORTD &= ~(1<<PD3); else PORTD |= (1<<PD3);
  if(eeprom_register[10]) PORTD &= ~(1<<PD6); else PORTD |= (1<<PD6);
  if(eeprom_register[11]) PORTD &= ~(1<<PD7); else PORTD |= (1<<PD7);
  if(eeprom_register[12]) PORTB &= ~(1<<PB0); else PORTB |= (1<<PB0);
  if(eeprom_register[13]) PORTB &= ~(1<<PB1); else PORTB |= (1<<PB1);
  if(eeprom_register[14]) PORTB &= ~(1<<PB2); else PORTB |= (1<<PB2);
  if(eeprom_register[15]) PORTB &= ~(1<<PB3); else PORTB |= (1<<PB3);
  if(eeprom_register[16]) PORTB &= ~(1<<PB4); else PORTB |= (1<<PB4);
  if(eeprom_register[17]) PORTB &= ~(1<<PB5); else PORTB |= (1<<PB5);
  
  DDRC |= (1<<PC0);
  DDRC |= (1<<PC1);
  DDRC |= (1<<PC2);
  DDRC |= (1<<PC3);
  DDRC |= (1<<PC4);
  DDRC |= (1<<PC5);
  DDRD |= (1<<PD2);
  DDRD |= (1<<PD3);
  DDRD |= (1<<PD6);
  DDRD |= (1<<PD7);
  DDRB |= (1<<PB0);
  DDRB |= (1<<PB1);
  DDRB |= (1<<PB2);
  DDRB |= (1<<PB3);
  DDRB |= (1<<PB4);
  DDRB |= (1<<PB5);
}
//-----------------------------------------------------------
void main (void)
{
   init_hardware();
   modbus_init();
   blink_init();
   timeout = 0;
   data_holding_register[31]= VERSION;
   __enable_interrupt();
   while(1)
   {
      if (INPUT1)  data_discret_inputs[0] |= (1<<0); else data_discret_inputs[0] &= ~(1<<0);
      if (INPUT2)  data_discret_inputs[0] |= (1<<1); else data_discret_inputs[0] &= ~(1<<1);
      if (INPUT3)  data_discret_inputs[0] |= (1<<2); else data_discret_inputs[0] &= ~(1<<2);
      if (INPUT4)  data_discret_inputs[0] |= (1<<3); else data_discret_inputs[0] &= ~(1<<3);
      if (INPUT5)  data_discret_inputs[0] |= (1<<4); else data_discret_inputs[0] &= ~(1<<4);
      if (INPUT6)  data_discret_inputs[0] |= (1<<5); else data_discret_inputs[0] &= ~(1<<5);
      if (INPUT7)  data_discret_inputs[0] |= (1<<6); else data_discret_inputs[0] &= ~(1<<6);
      if (INPUT8)  data_discret_inputs[0] |= (1<<7); else data_discret_inputs[0] &= ~(1<<7);

      if (INPUT9)  data_discret_inputs[1] |= (1<<0); else data_discret_inputs[1] &= ~(1<<0);
      if (INPUT10) data_discret_inputs[1] |= (1<<1); else data_discret_inputs[1] &= ~(1<<1);
      if (INPUT11) data_discret_inputs[1] |= (1<<2); else data_discret_inputs[1] &= ~(1<<2);
      if (INPUT12) data_discret_inputs[1] |= (1<<3); else data_discret_inputs[1] &= ~(1<<3);
      if (INPUT13) data_discret_inputs[1] |= (1<<4); else data_discret_inputs[1] &= ~(1<<4);
      if (INPUT14) data_discret_inputs[1] |= (1<<5); else data_discret_inputs[1] &= ~(1<<5);
      if (INPUT15) data_discret_inputs[1] |= (1<<6); else data_discret_inputs[1] &= ~(1<<6);
      if (INPUT16) data_discret_inputs[1] |= (1<<7); else data_discret_inputs[1] &= ~(1<<7);

       if (timeout == 0)
       {
          if(!eeprom_register[2])  
            PORTC &= ~(1<<PC0); 
          else 
            PORTC |= (1<<PC0);
          
          if(!eeprom_register[3])  
            PORTC &= ~(1<<PC1); 
          else 
            PORTC |= (1<<PC1);
          
          if(!eeprom_register[4])  
            PORTC &= ~(1<<PC2); 
          else 
            PORTC |= (1<<PC2);
          
          if(!eeprom_register[5])  
            PORTC &= ~(1<<PC3); 
          else 
            PORTC |= (1<<PC3);
          
          if(!eeprom_register[6])  
            PORTC &= ~(1<<PC4); 
          else 
            PORTC |= (1<<PC4);
          
          if(!eeprom_register[7])  
            PORTC &= ~(1<<PC5); 
          else 
            PORTC |= (1<<PC5);
          
          if(!eeprom_register[8])  
            PORTD &= ~(1<<PD2); 
          else 
            PORTD |= (1<<PD2);
          
          if(!eeprom_register[9])  
            PORTD &= ~(1<<PD3); 
          else 
            PORTD |= (1<<PD3);
          
          if(!eeprom_register[10])  
            PORTD &= ~(1<<PD6);
          else 
            PORTD |= (1<<PD6);
          
          if(!eeprom_register[11])  
            PORTD &= ~(1<<PD7); 
          else 
            PORTD |= (1<<PD7);
          
          if(!eeprom_register[12]) 
            PORTB &= ~(1<<PB0); 
          else 
            PORTB |= (1<<PB0);
          
          if(!eeprom_register[13]) 
            PORTB &= ~(1<<PB1);
          else 
            PORTB |= (1<<PB1);
          
          if(!eeprom_register[14]) 
            PORTB &= ~(1<<PB2);
          else 
            PORTB |= (1<<PB2);
          
          if(!eeprom_register[15]) 
            PORTB &= ~(1<<PB3);
          else 
            PORTB |= (1<<PB3);
          
          if(!eeprom_register[16]) 
            PORTB &= ~(1<<PB4);
          else 
            PORTB |= (1<<PB4);
          
          if(!eeprom_register[17]) 
            PORTB &= ~(1<<PB5);
          else 
            PORTB |= (1<<PB5);
       }
       else
       {
           if (data_coil_register[0] & (1<<0)) 
           {
             if(eeprom_register[2])
               OUTPUT1_0;
             else  
               OUTPUT1_1;
           }
           else
           {
             if(eeprom_register[2])
               OUTPUT1_1;
             else  
               OUTPUT1_0;
           }
           
           if (data_coil_register[0] & (1<<1)) 
           {
             if(eeprom_register[3])
              OUTPUT2_0;
             else  
              OUTPUT2_1;
           }
           else
           {
             if(eeprom_register[3])
              OUTPUT2_1;
             else  
              OUTPUT2_0;
           }
           
           if (data_coil_register[0] & (1<<2)) 
           {
             if(eeprom_register[4])
              OUTPUT3_0;
             else  
              OUTPUT3_1;
           }
           else
           {
             if(eeprom_register[4])
              OUTPUT3_1;
             else  
              OUTPUT3_0;
           }
           
           if (data_coil_register[0] & (1<<3)) 
           {
             if(eeprom_register[5])
              OUTPUT4_0;
             else  
              OUTPUT4_1;
           }
           else
           {
             if(eeprom_register[5])
              OUTPUT4_1;
             else  
              OUTPUT4_0;
           }
           
           if (data_coil_register[0] & (1<<4)) 
           {
             if(eeprom_register[6])
              OUTPUT5_0;
             else  
              OUTPUT5_1;
           }
           else
           {
             if(eeprom_register[6])
              OUTPUT5_1;
             else  
              OUTPUT5_0;
           }
           
           if (data_coil_register[0] & (1<<5)) 
           {
             if(eeprom_register[7])
              OUTPUT6_0;
             else  
              OUTPUT6_1;
           }
           else
           {
             if(eeprom_register[7])
              OUTPUT6_1;
             else  
              OUTPUT6_0;
           }
           
           if (data_coil_register[0] & (1<<6)) 
           {
             if(eeprom_register[8])
              OUTPUT7_0;
             else  
              OUTPUT7_1;
           }
           else
           {
             if(eeprom_register[8])
              OUTPUT7_1;
             else  
              OUTPUT7_0;
           }
             
           if (data_coil_register[0] & (1<<7)) 
           {
             if(eeprom_register[9])
              OUTPUT8_0;
             else  
              OUTPUT8_1;
           }
           else
           {
             if(eeprom_register[9])
              OUTPUT8_1;
             else  
              OUTPUT8_0;
           }
           
           if (data_coil_register[1] & (1<<0)) 
           {
             if(eeprom_register[10])
              OUTPUT9_0;
             else  
              OUTPUT9_1;
           }
           else
           {
             if(eeprom_register[10])
              OUTPUT9_1;
             else  
              OUTPUT9_0;
           }
           
           if (data_coil_register[1] & (1<<1)) 
           {
             if(eeprom_register[11])
              OUTPUT10_0;
             else 
              OUTPUT10_1;
           }
           else
           {
             if(eeprom_register[11])
              OUTPUT10_1;
             else 
              OUTPUT10_0;
           }
           
           if (data_coil_register[1] & (1<<2)) 
           {
             if(eeprom_register[12])
              OUTPUT11_0;
             else 
              OUTPUT11_1;
           }
           else
           {
             if(eeprom_register[12])
              OUTPUT11_1;
             else 
              OUTPUT11_0;
           }
           
           if (data_coil_register[1] & (1<<3)) 
           {
             if(eeprom_register[13])
              OUTPUT12_0;
             else 
              OUTPUT12_1;
           }
           else
           {
             if(eeprom_register[13])
              OUTPUT12_1;
             else 
              OUTPUT12_0;
           }
           
           if (data_coil_register[1] & (1<<4))
           {
             if(eeprom_register[14])
              OUTPUT13_0;
             else 
              OUTPUT13_1;
           }
           else
           {
             if(eeprom_register[14])
              OUTPUT13_1;
             else 
              OUTPUT13_0;
           }
           
           if (data_coil_register[1] & (1<<5)) 
           {
             if(eeprom_register[15])
              OUTPUT14_0;
             else 
              OUTPUT14_1;
           }
           else
           {
             if(eeprom_register[15])
              OUTPUT14_1;
             else 
              OUTPUT14_0;
           }
           
           if (data_coil_register[1] & (1<<6)) 
           {
             if(eeprom_register[16])
              OUTPUT15_0;
             else 
              OUTPUT15_1;
           }
           else
           {
             if(eeprom_register[16])
              OUTPUT15_1;
             else 
              OUTPUT15_0;
           }
           
           if (data_coil_register[1] & (1<<7)) 
           {
             if(eeprom_register[17])
              OUTPUT16_0;
             else 
              OUTPUT16_1;
           }
           else
           {
             if(eeprom_register[17])
              OUTPUT16_1;
             else 
              OUTPUT16_0;
           }
       }
   }
}

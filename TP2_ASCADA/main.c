/*
 * main.c
 *  ------------------------------------------ DINPUTS ---------------------------------------------------
 *  Created on: 08.04.2012
 *      Author: TCom
 */
#include "LIB_SH\main.h"
#include "LIB_SH\LIBS_MODBUS\modbus.h"
#include "LIB_SH\LIB_BLINK\blink.h"
#include "hardware.h"

#define VERSION 10105
//-----------------------------------------------------------
typedef enum x{off,blink1,blink2,on} state;
extern __no_init unsigned int __eeprom eeprom_register[128];
state blk[8]={off,off,off,off,off,off,off,off};
//-----------------------------------------------------------
void init_hardware(void)
{
	PORTC = 0x00;
	DDRC = 0x00;

	PORTB = 0xFF;
	DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5);

	PORTD = (1<<PD2)|(1<<PD3);
	DDRD =  (1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);
}
//-----------------------------------------------------------
unsigned int  ReadADC (unsigned char channel)    
{  
	ADMUX = channel|(1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	while (!(ADCSRA & (1<<ADIF)));
	ACSR |= 0x10;
	return ADC;
} 
//-----------------------------------------------------------
void timer2_init(void)
{
	/*
	 * CPU_CLK_Hz 18432000
	 *   tick timer = 18432000/1024 = 18000 ??? ? ???????
	 *   18000 / 180 = 100  ???? ? ???????
     */
	TIMSK2 = (1<<OCIE2A);
	OCR2A = 180;
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
}

void led_control(unsigned char led,bool en)
{
 switch(led)
 {
   case 0: if(en) OUTPUT1_1; else OUTPUT1_0; return;
   case 1: if(en) OUTPUT2_1; else OUTPUT2_0; return;
   case 2: if(en) OUTPUT3_1; else OUTPUT3_0; return;
   case 3: if(en) OUTPUT4_1; else OUTPUT4_0; return;
   case 4: if(en) OUTPUT5_1; else OUTPUT5_0; return;
   case 5: if(en) OUTPUT6_1; else OUTPUT6_0; return;
   case 6: if(en) OUTPUT7_1; else OUTPUT7_0; return;
   case 7: if(en) OUTPUT8_1; else OUTPUT8_0; return;
 }
}

#define BLINK1 50
#define BLINK2 10

#pragma vector = TIMER2_COMPA_vect
__interrupt void timer2_event (void) // 0,01 C
{
  __enable_interrupt();
  static unsigned int interval1=0,interval2=0;
  static  bool togle1 = false,togle2=false;
  unsigned char i = 0;
  
  if(interval1++ > BLINK1)
  {
    if(togle1)togle1 = false;else togle1 = true; 
    interval1 = 0;
  }
  
  if(interval2++ > BLINK2)
  {
    if(togle2)togle2 = false;else togle2 = true; 
    interval2 = 0;
  }
  
  for(i=0;i<8;i++)
  {
    if(togle1)
    {
        if(blk[i] == blink1) led_control(i,true);
    }
    else
    {
        if(blk[i] == blink1) led_control(i,false);
    }
  }
  
  for(i=0;i<8;i++)
   {
     if(togle2)
     {
         if(blk[i] == blink2) led_control(i,true);
     }
     else
     {
         if(blk[i] == blink2) led_control(i,false);
     }
   }

  for(i=0;i<8;i++)
   {
         if(blk[i] == on) led_control(i,true);
         if(blk[i] == off) led_control(i,false);
   }
  
  TCNT2 = 0; 
}
//-----------------------------------------------------------
void main (void)
{
   init_hardware();
   modbus_init();
   blink_init();
   timer2_init();
   data_holding_register[31]= VERSION;
   __enable_interrupt();
  int i = 0;
  while(1)
  {
    for (i=0;i<8;i++)
    {
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      ReadADC(i);
      delay_ms(10);
      data_input_register[i] = ReadADC(i);
    }

    //------------------------------------------------------------
    int reg=2;
    for(i=0;i<8;i++)
    {
        if(data_input_register[i] < eeprom_register[reg]) blk[i] = off;
        else
        {
            if(data_input_register[i] < eeprom_register[reg+1])
              blk[i] = blink1;
            else
            {
                if(data_input_register[i] < eeprom_register[reg+2])
                  blk[i] = blink2;
                else
                  blk[i] = on;
            }
        }
        reg+=3;
    }

  }
}

/*
 * main.c
 *  ------------------------------------------ DINPUTS ---------------------------------------------------
 *  Created on: 08.04.2012
 *      Author: TCom
 */
#include "LIB_SH\main.h"
#include "LIB_SH\LIBS_MODBUS\modbus.h"
#include "LIB_SH\LIB_BLINK\blink.h"
#include "LIB_SH\LIBS_ONEWIRE\onewire.h"
#include "LIB_SH\LIBS_ONEWIRE\ds18x20.h"
#include "hardware.h"
#include <string.h>



#define VERSION 10118
//------------------------------------------------------------
extern __no_init unsigned int __eeprom eeprom_register[32];
//-----------------------------------------------------------
typedef struct
{
	float temperature;
	bool connected;
}_sensor;

_sensor sensor[16];


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
typedef struct
{
    volatile unsigned char* in;
	volatile unsigned char* out;
	volatile unsigned char* ddr;
	unsigned char pin;
}_dt_pin;

_dt_pin PinOut[] =
{	// in    out   ddr pin
	&PINC,&PORTC,&DDRC,PC0,
	&PINC,&PORTC,&DDRC,PC1,
	&PINC,&PORTC,&DDRC,PC2,
	&PINC,&PORTC,&DDRC,PC3,
	&PINC,&PORTC,&DDRC,PC4,
	&PINC,&PORTC,&DDRC,PC5,

	&PIND,&PORTD,&DDRD,PD2,
	&PIND,&PORTD,&DDRD,PD3,
	&PIND,&PORTD,&DDRD,PD6,
	&PIND,&PORTD,&DDRD,PD7,

	&PINB,&PORTB,&DDRB,PB0,
	&PINB,&PORTB,&DDRB,PB1,
	&PINB,&PORTB,&DDRB,PB2,
	&PINB,&PORTB,&DDRB,PB3,
	&PINB,&PORTB,&DDRB,PB4,
	&PINB,&PORTB,&DDRB,PB5
};
//----------------------------------------------------------
void start_convert(void)
{
	unsigned char i = 0;
	unsigned char subzero, cel, cel_frac_bits;

	int decicelsius;
	for(i=0;i<16;i++)
	{
		ow_set_bus(PinOut[i].in,PinOut[i].out,PinOut[i].ddr,PinOut[i].pin);
		if (DS18X20_start_meas(DS18X20_POWER_EXTERN, 0)== DS18X20_OK)
			sensor[i].connected = true;
		else
			sensor[i].connected = false;
	}

	delay_ms(DS18B20_TCONV_12BIT);

	for(i=0;i<16;i++)
	{
		if(sensor[i].connected)
		{
			ow_set_bus(PinOut[i].in,PinOut[i].out,PinOut[i].ddr,PinOut[i].pin);
			if (DS18X20_read_meas_single(((unsigned char)eeprom_register[i+2]), &subzero, &cel,&cel_frac_bits) == DS18X20_OK)
			{
				decicelsius = DS18X20_temp_to_decicel(subzero, cel, cel_frac_bits);
			}
			else 
                        {
                          sensor[i].connected = false;
                          continue;
                        }
			if (subzero == 1) decicelsius = decicelsius * (-1);
			sensor[i].temperature = decicelsius*0.1;
		}

	}
}
//----------------------------------------------------------
void main (void)
{
   init_hardware();
   modbus_init();
   blink_init();
   unsigned char i = 0,a=0;
   data_holding_register[31]= VERSION;
   __enable_interrupt();

   while(1)
   {
	   start_convert();
           for(i=0;i<16;i++)
           {
             if(!sensor[i].connected) sensor[i].temperature = -200;
             memcpy(data_input_register+(i*2),&sensor[i].temperature,sizeof(float));
           }
   }
}

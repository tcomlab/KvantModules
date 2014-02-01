/*********************************************************************************

changelog:
20041124 - Extended measurements for DS18(S)20 contributed by Carsten Foss (CFO)
200502xx - function DS18X20_read_meas_single
20050310 - DS18x20 EEPROM functions (can be disabled to save flash-memory)
           (DS18X20_EEPROMSUPPORT in ds18x20.h)

**********************************************************************************/
#include "onewire.h"
#include "ds18x20.h"

#include "crc8.h"

unsigned char
DS18X20_meas_to_cel(unsigned char fc, unsigned char *sp, unsigned char* subzero,
    unsigned char* cel, unsigned char* cel_frac_bits)
{
  unsigned int meas;
  unsigned char i;

  meas = sp[0];  // LSB
  meas |= ((unsigned int) sp[1]) << 8; // MSB
  //meas = 0xff5e; meas = 0xfe6f;
  __watchdog_reset();
  //  only work on 12bit-base
  if (fc == DS18S20_ID)
    { // 9 -> 12 bit if 18S20
      /* Extended measurements for DS18S20 contributed by Carsten Foss */
      meas &= (unsigned int) 0xfffe;// Discard LSB , needed for later extended precicion calc
      meas <<= 3;		// Convert to 12-bit , now degrees are in 1/16 degrees units
      meas += (16 - sp[6]) - 4;	// Add the compensation , and remember to subtract 0.25 degree (4/16)
    }
  // check for negative
  if (meas & 0x8000)
    {
      *subzero = 1;      // mark negative
      meas ^= 0xffff;  // convert to positive => (twos complement)++
      meas++;
    }
  else
    *subzero = 0;

  // clear undefined bits for B != 12bit
  if (fc == DS18B20_ID)
    { // check resolution 18B20
      __watchdog_reset();
      i = sp[DS18B20_CONF_REG];
      if ((i & DS18B20_12_BIT)== DS18B20_12_BIT )
        ;
      else if ((i & DS18B20_11_BIT)== DS18B20_11_BIT )
        meas &= ~(DS18B20_11_BIT_UNDF);
      else if ((i & DS18B20_10_BIT)== DS18B20_10_BIT )
        meas &= ~(DS18B20_10_BIT_UNDF);
      else
        // if ( (i & DS18B20_9_BIT) == DS18B20_9_BIT ) {
        meas &= ~(DS18B20_9_BIT_UNDF);
    }

  *cel = (unsigned char) (meas >> 4);
  *cel_frac_bits = (unsigned char) (meas & 0x000F);
  return DS18X20_OK;
}

/* converts to decicelsius
   input is ouput from meas_to_cel
   returns absolute value of temperatur in decicelsius
	i.e.: sz=0, c=28, frac=15 returns 289 (=28.9°C)
0	0	0	
1	625	625	1
2	1250	250	
3	1875	875	3
4	2500	500	4
5	3125	125	
6	3750	750	6
7	4375	375	
8	5000	0	
9	5625	625	9
10	6250	250	
11	6875	875	11
12	7500	500	12
13	8125	125	
14	8750	750	14
15	9375	375	*/
unsigned int DS18X20_temp_to_decicel(unsigned char subzero, unsigned char cel, 
   unsigned char cel_frac_bits)

 { unsigned int h;
   unsigned char  i;
   unsigned char need_rounding[] = { 1, 3, 4, 6, 9, 11, 12, 14 };
  __watchdog_reset();
   h = cel_frac_bits*DS18X20_FRACCONV/1000;
   h += cel*10;
   if (!subzero) 
    { for (i=0; i<sizeof(need_rounding); i++) 
       { if ( cel_frac_bits == need_rounding[i] ) 
          { h++;
            break;
          }
       }
    }
   return h;
 }

/* find DS18X20 Sensors on 1-Wire-Bus
   input/ouput: diff is the result of the last rom-search
   output: id is the rom-code of the sensor found */
void DS18X20_find_sensor(unsigned char *diff, unsigned char id[])
 { for (;;) //<<<<<<<<<<<<<---------------------------------------------~!
    { *diff = ow_rom_search( *diff, &id[0] );
      if ( *diff==OW_PRESENCE_ERR || *diff==OW_DATA_ERR ||
      *diff == OW_LAST_DEVICE ) return;
      if ( id[0] == DS18B20_ID || id[0] == DS18S20_ID ) return;
    }
 }



/* start measurement (CONVERT_T) for all sensors if input id==NULL 
   or for single sensor. then id is the rom-code */
unsigned char DS18X20_start_meas( unsigned char with_power_extern, unsigned char id[])
 { ow_reset(); //**
   if( ow_input_pin_state() ) 
    { // only send if bus is "idle" = high
      ow_command( DS18X20_CONVERT_T, id );
      if (with_power_extern != DS18X20_POWER_EXTERN)
         ow_parasite_enable();
      __watchdog_reset();
        return DS18X20_OK;
    } 
   else 
    { //
      #ifdef DS18X20_VERBOSE
          //lcd_put_S( "Short Circuit !\n",2 );
      #endif
      return DS18X20_START_FAIL;
    }
 }

/* reads temperature (scratchpad) of sensor with rom-code id
   output: subzero==1 if temp.<0, cel: full celsius, mcel: frac 
   in millicelsius*0.1
   i.e.: subzero=1, cel=18, millicel=5000 = -18,5000°C */
unsigned char DS18X20_read_meas(unsigned char id[], unsigned char *subzero, 
   unsigned char *cel, unsigned char *cel_frac_bits)

 { unsigned char i;
   unsigned char sp[DS18X20_SP_SIZE];
    __watchdog_reset();
   ow_reset(); //**
   ow_command(DS18X20_READ, id);
   for ( i=0 ; i< DS18X20_SP_SIZE; i++ ) sp[i]=ow_byte_rd();
   if ( crc8( &sp[0], DS18X20_SP_SIZE ) ) 
      return DS18X20_ERROR_CRC;
   DS18X20_meas_to_cel(id[0], sp, subzero, cel, cel_frac_bits);
   return DS18X20_OK;
 }

/* reads temperature (scratchpad) of a single sensor (uses skip-rom)
   output: subzero==1 if temp.<0, cel: full celsius, mcel: frac 
   in millicelsius*0.1
   i.e.: subzero=1, cel=18, millicel=5000 = -18,5000°C */
unsigned char DS18X20_read_meas_single(unsigned char familycode, unsigned char *subzero, 
   unsigned char *cel, unsigned char *cel_frac_bits)

 { unsigned char i;
   unsigned char sp[DS18X20_SP_SIZE];
    __watchdog_reset();
   ow_command(DS18X20_READ, NULL);
   for ( i=0 ; i< DS18X20_SP_SIZE; i++ ) sp[i]=ow_byte_rd();
   if ( crc8( &sp[0], DS18X20_SP_SIZE ) ) 
      return DS18X20_ERROR_CRC;
   DS18X20_meas_to_cel(familycode, sp, subzero, cel, cel_frac_bits);
   return DS18X20_OK;
 }


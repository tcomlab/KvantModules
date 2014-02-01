
#include "onewire.h"
/*
#define OW_PIN PC0 //sensor[curent_selected_port].BIT // PD4 
#define OW_IN  PINC //sensor[curent_selected_port].PIN // PIND
#define OW_OUT PORTC //sensor[curent_selected_port].PORT // PORTD
#define OW_DDR DDRC //sensor[curent_selected_port].DDR // DDRD

#define OW_GET_IN()   ( OW_IN & (1<<OW_PIN))
#define OW_OUT_LOW()  ( OW_OUT &= (~(1 << OW_PIN)) )
#define OW_OUT_HIGH() ( OW_OUT |= (1 << OW_PIN) )
#define OW_DIR_IN()   ( OW_DDR &= (~(1 << OW_PIN )) )
#define OW_DIR_OUT()  ( OW_DDR |= (1 << OW_PIN) )

extern unsigned char curent_selected_port;
*/

/* set bus-config with ow_set_bus() */
unsigned char OW_PIN_MASK; 
volatile unsigned char* OW_IN;
volatile unsigned char* OW_OUT;
volatile unsigned char* OW_DDR;

#define OW_GET_IN()   ( *OW_IN & OW_PIN_MASK )
#define OW_OUT_LOW()  ( *OW_OUT &= (unsigned char) ~OW_PIN_MASK )
#define OW_OUT_HIGH() ( *OW_OUT |= (unsigned char)  OW_PIN_MASK )
#define OW_DIR_IN()   ( *OW_DDR &= (unsigned char) ~OW_PIN_MASK )
#define OW_DIR_OUT()  ( *OW_DDR |= (unsigned char)  OW_PIN_MASK )

void ow_set_bus(volatile unsigned char* in,
	volatile unsigned char* out,
	volatile unsigned char* ddr,
	unsigned char pin)
{
	OW_DDR=ddr;
	OW_OUT=out;
	OW_IN=in;
	OW_PIN_MASK = (1 << pin);
	ow_reset();
}


unsigned char ow_input_pin_state()
 { return OW_GET_IN();
 }

void ow_parasite_enable(void)
 { OW_OUT_HIGH();
   OW_DIR_OUT();
 }

//void ow_parasite_disable(void)
// { OW_OUT_LOW();
//   OW_DIR_IN();
// }

unsigned char ow_reset(void)
 { unsigned char err;
   OW_OUT_LOW(); // disable internal pull-up (maybe on from parasite)
   OW_DIR_OUT(); // pull OW-Pin low for 480us

   delay_us(480);
   __disable_interrupt();

   // set Pin as input - wait for clients to pull low
   OW_DIR_IN(); // input
   delay_us(66);
   err = OW_GET_IN();		// no presence detect
   // nobody pulled to low, still high

    __enable_interrupt();

   // after a delay the clients should release the line
   // and input-pin gets back to high due to pull-up-resistor
   delay_us(414);
   if( OW_GET_IN() == 0 )		// short circuit
      err = 1;
   return err;
 }

/* Timing issue when using runtime-bus-selection (!OW_ONE_BUS):
   The master should sample at the end of the 15-slot after initiating
   the read-time-slot. The variable bus-settings need more
   cycles than the constant ones so the delays had to be shortened 
   to achive a 15uS overall delay 
   Setting/clearing a bit in I/O Register needs 1 cyle in OW_ONE_BUS
   but around 14 cyles in configureable bus (us-Delay is 4 cyles per uS) */
unsigned char ow_bit_io( unsigned char b )
 {//unsigned char sreg;

   //sreg=SREG;
   __disable_interrupt();
   __watchdog_reset();
	
   OW_DIR_OUT(); // drive bus low

   delay_us(1); // Recovery-Time wuffwuff was 1
   if ( b ) OW_DIR_IN(); // if bit is 1 set bus high (by ext. pull-up)
	
   // wuffwuff delay was 15uS-1 see comment above
   delay_us(15-1-OW_CONF_DELAYOFFSET);
      
   if( OW_GET_IN() == 0 ) b = 0;  // sample at end of read-timeslot
	
   delay_us(45);
   OW_DIR_IN();
   
  __enable_interrupt();
	
   return b;
 }


unsigned char ow_byte_wr( unsigned char b )
 { unsigned char i = 8, j;
   do 
    { j = ow_bit_io( b & 1 );
      b >>= 1;
      if( j ) b |= 0x80;
      __watchdog_reset();
    } 
   while( --i );
   return b;
 }


unsigned char ow_byte_rd( void )
 { // read by sending 0xff (a dontcare?)
   return ow_byte_wr( 0xFF ); 
 }


unsigned char ow_rom_search( unsigned char diff, unsigned char *id )
 { unsigned char i, j, next_diff;
   unsigned char b;
__watchdog_reset();
   if( ow_reset() ) 
      return OW_PRESENCE_ERR;       // error, no device found

   ow_byte_wr( OW_SEARCH_ROM );     // ROM search command
   next_diff = OW_LAST_DEVICE;      // unchanged on last device

   i = OW_ROMCODE_SIZE * 8;         // 8 bytes
   do 
    {	j = 8;                        // 8 bits
       __watchdog_reset();
		do 
       { b = ow_bit_io( 1 );			// read bit
         if( ow_bit_io( 1 ) ) 
          { // read complement bit
            if( b )                 // 11
               return OW_DATA_ERR;  // data error
          }
         else 
          { if( !b ) 
             { // 00 = 2 devices
               if( diff > i || ((*id & 1) && diff != i) ) 
                { b = 1;               // now 1
                  next_diff = i;       // next pass 0
                  __watchdog_reset();
                }
             }
          }
         ow_bit_io( b );               // write bit
         *id >>= 1;
         if( b ) *id |= 0x80;			// store bit
         i--;
       } 
      while( --j );
      id++;                            // next byte
    } 
   while( i );
   return next_diff;                  // to continue search
 }


void ow_command( unsigned char command, unsigned char *id )
 { unsigned char i;

   ow_reset();
   if( id ) 
    { ow_byte_wr( OW_MATCH_ROM );			// to a single device
      i = OW_ROMCODE_SIZE;
      do 
       { ow_byte_wr( *id );
         id++;
         __watchdog_reset();
       } 
      while( --i );
    } 
   else
      ow_byte_wr( OW_SKIP_ROM );			// to all devices

   ow_byte_wr( command );
 }

#ifndef _1wire_h_
#define _1wire_h_


#include "LIB_SH\main.h"

/*******************************************/
/* Hardware connection                     */
/*******************************************/

/* Define OW_ONE_BUS if only one 1-Wire-Bus is used
   in the application -> shorter code.
   If not defined make sure to call ow_set_bus() before using 
   a bus. Runtime bus-select increases code size by around 300 
   bytes so use OW_ONE_BUS if possible */
#define OW_ONE_BUS




#ifdef OW_ONE_BUS

//#define OW_PIN  PD4
//#define OW_IN   PIND
//#define OW_OUT  PORTD
//#define OW_DDR  DDRD


#define OW_CONF_DELAYOFFSET 0

#else 
#if CPU_FREQ<1843200
 #warning | experimental multi-bus-mode is not tested for 
 #warning | frequencies below 1,84MHz - use OW_ONE_WIRE or
 #warning | faster clock-source (i.e. internal 2MHz R/C-Osc.)
#endif
#define OW_CONF_CYCLESPERACCESS 13
#define OW_CONF_DELAYOFFSET ( (unsigned int)( ((OW_CONF_CYCLESPERACCESS)*1000000L) / CPU_FREQ  ) )
#endif

/*******************************************/

// #define OW_SHORT_CIRCUIT  0x01

#define OW_MATCH_ROM	0x55
#define OW_SKIP_ROM	    0xCC
#define	OW_SEARCH_ROM	0xF0

#define	OW_SEARCH_FIRST	0xFF		// start new search
#define	OW_PRESENCE_ERR	0xFF
#define	OW_DATA_ERR	    0xFE
#define OW_LAST_DEVICE	0x00		// last device found
//			0x01 ... 0x40: continue searching

// rom-code size including CRC
#define OW_ROMCODE_SIZE 8

extern unsigned char ow_reset(void);

extern unsigned char ow_bit_io( unsigned char b );
extern unsigned char ow_byte_wr( unsigned char b );
extern unsigned char ow_byte_rd( void );

extern unsigned char ow_rom_search( unsigned char diff, unsigned char *id );

extern void ow_command( unsigned char command, unsigned char *id );

extern void ow_parasite_enable(void);
extern void ow_parasite_disable(void);
extern unsigned char ow_input_pin_state(void);

extern void ow_set_bus(volatile unsigned char* in,
	volatile unsigned char* out,
	volatile unsigned char* ddr,
	unsigned char pin);

#endif


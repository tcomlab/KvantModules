#ifndef DS18X20_H_
#define DS18X20_H_

#include <stdlib.h>

// enable DS18x20 EERPROM support
#define DS18X20_EEPROMSUPPORT

// enable extended output via UART by defining:
#define DS18X20_VERBOSE

/* return values */
#define DS18X20_OK          0x00
#define DS18X20_ERROR       0x01
#define DS18X20_START_FAIL  0x02
#define DS18X20_ERROR_CRC   0x03

#define DS18X20_POWER_PARASITE 0x00
#define DS18X20_POWER_EXTERN   0x01

/* DS18X20 specific values (see datasheet) */
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28

#define DS18X20_CONVERT_T	0x44
#define DS18X20_READ		0xBE
#define DS18X20_WRITE		0x4E
#define DS18X20_EE_WRITE	0x48
#define DS18X20_EE_RECALL	0xB8
#define DS18X20_READ_POWER_SUPPLY 0xB4

#define DS18B20_CONF_REG    4
#define DS18B20_9_BIT       0
#define DS18B20_10_BIT      (1<<5)
#define DS18B20_11_BIT      (1<<6)
#define DS18B20_12_BIT      ((1<<6)|(1<<5))

// indefined bits in LSB if 18B20 != 12bit
#define DS18B20_9_BIT_UNDF       ((1<<0)|(1<<1)|(1<<2))
#define DS18B20_10_BIT_UNDF      ((1<<0)|(1<<1))
#define DS18B20_11_BIT_UNDF      ((1<<0))
#define DS18B20_12_BIT_UNDF      0

// conversion times in ms
#define DS18B20_TCONV_12BIT      750
#define DS18B20_TCONV_11BIT      DS18B20_TCONV_12_BIT/2
#define DS18B20_TCONV_10BIT      DS18B20_TCONV_12_BIT/4
#define DS18B20_TCONV_9BIT       DS18B20_TCONV_12_BIT/8
#define DS18S20_TCONV            DS18B20_TCONV_12_BIT

// constant to convert the fraction bits to cel*(10^-4)
#define DS18X20_FRACCONV         625

#define DS18X20_SP_SIZE  9

// DS18X20 EEPROM-Support
#define DS18X20_WRITE_SCRATCHPAD  0x4E
#define DS18X20_COPY_SCRATCHPAD   0x48
#define DS18X20_RECALL_E2         0xB8
#define DS18X20_COPYSP_DELAY      10 /* ms */
#define DS18X20_TH_REG      2
#define DS18X20_TL_REG      3


/* for description of functions see ds18x20.c */

extern void DS18X20_find_sensor(unsigned char *diff, unsigned char id[]);
	
extern unsigned char	DS18X20_get_power_status(unsigned char id[]);

extern unsigned char DS18X20_start_meas( unsigned char with_external, unsigned char id[]);
extern unsigned char DS18X20_read_meas(unsigned char id[], unsigned char *subzero, unsigned char *cel, unsigned char *cel_frac_bits);
extern unsigned char DS18X20_read_meas_single(unsigned char familycode, unsigned char *subzero, unsigned char *cel, unsigned char *cel_frac_bits);
extern unsigned char DS18X20_meas_to_cel( unsigned char fc, unsigned char *sp, unsigned char* subzero, unsigned char* cel, unsigned char* cel_frac_bits);
extern	unsigned int DS18X20_temp_to_decicel(unsigned char subzero, unsigned char cel, unsigned char cel_frac_bits);
extern char DS18X20_temp_cmp(unsigned char subzero1, unsigned int cel1, unsigned char subzero2, unsigned int cel2);

#ifdef DS18X20_EEPROMSUPPORT
// write th, tl and config-register to scratchpad (config ignored on S20)
unsigned char DS18X20_write_scratchpad( unsigned char id[], unsigned char th, unsigned char tl, unsigned char conf);
// read scratchpad into array SP
unsigned char DS18X20_read_scratchpad( unsigned char id[], unsigned char sp[]);
// copy values th,tl (config) from scratchpad to DS18x20 eeprom
unsigned char DS18X20_copy_scratchpad( unsigned char with_power_extern, unsigned char id[] );
// copy values from DS18x20 eeprom to scratchpad
unsigned char DS18X20_recall_E2( unsigned char id[] );
#endif

#ifdef DS18X20_VERBOSE
extern void DS18X20_show_id_uart( unsigned char *id, size_t n );
extern unsigned char DS18X20_read_meas_all_verbose( void );
#endif

#endif

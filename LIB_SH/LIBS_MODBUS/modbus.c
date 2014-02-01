/*
 * modbus.c
 *
 *  Created on: 08.04.2012
 *      Author: TCom
 */
//---------------------------------------------------------------
#include "LIB_SH\main.h"
#include "modbus.h"
#include <string.h>
#include "modbus_crc.h"
#include "LIB_SH\LIB_BLINK\blink.h"
#include <ina90.h>
#define VERSION 3.08.01
//---------------------------------------------------------------
#define device_adress   eeprom_register[0]
#define device_baudrate eeprom_register[1]

__no_init unsigned int __eeprom eeprom_register[32];
//---------------------------------------------------------------
unsigned int rx_counter = 0;
unsigned char rx_buff[64];
unsigned char tmp_adress = 0;
//unsigned int start_state = 0;
//---------------------------------------------------------------
// modbus data register
 unsigned char data_coil_register[8];                     // CMD = 0x01
 unsigned char data_discret_inputs[8];           // CMD = 0x02
 unsigned int  data_holding_register[32]; // CMD = 0x03
 unsigned int  data_input_register[32];          // CMD = 0x04

//----------------------------------------------------------------------------
void send_modbus_bit(unsigned char cmd,unsigned char data_quantity_byte,unsigned char *data);
unsigned int delay_tx = 0;
//---------------------------------------------------------------
void tx_delay()
{
  switch(delay_tx)
  {
    case 0x00EF: // 4800
      delay_ms(2);
      return;
    case 0x0077: // 9600
      delay_ms(1);
      return;  
    case 0x003b: // 19200
      delay_us(1000);
      return;
    case 0x001d: // 38400
      delay_us(800);
      return;
    case 0x0013: // 57400
      delay_us(400);
      return;
    case 0x0009: // 115200
      delay_us(200);
      return;
  }
}
//---------------------------------------------------------------
void remix_data(unsigned char *dest,int lengh )
{
  int i = 0,p=0,k=0;
  for (i=0;i<lengh;i++) 
  {
    k = dest[p];
    dest[p] = dest[p+1];
    dest[p+1] = k;
    p += 2;
  }
}
//---------------------------------------------------------------
void SetUBR(void)
{
  switch(device_baudrate)
  {
    case 0: // 4800
      UBRR0H = 0;
      UBRR0L = 239;
    break;
    
    case 1: // 9600
      UBRR0H = 0;
      UBRR0L = 119;
    break;
    
    case 2: // 19200
      UBRR0H = 0;
      UBRR0L = 59;
    break;
    
    case 3: // 38400
      UBRR0H = 0;
      UBRR0L = 29;
    break;
    
    case 4: // 57600
      UBRR0H = 0;
      UBRR0L = 19;
    break;
    
    case 5: // 115200
      UBRR0H = 0;
      UBRR0L = 9;
    break;
    
  default:
      UBRR0H = 0;
      UBRR0L = 9;
    break;
  }
  delay_tx = UBRR0L;
}

void modbus_init(void)
{
  SetUBR();
  UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
  UCSR0C = (3<<UCSZ00);
  OCR0A = 8;
  TIMSK0 |= (1<<OCIE0A);
  rx_counter = 0;
  RX_TX0;
}
//---------------------------------------------------------------
#pragma vector = USART_RX_vect
__interrupt void recive_byte (void)
{
  //__enable_interrupt();
  if(rx_counter >= sizeof(rx_buff)) rx_counter = 0;
  rx_buff[rx_counter++] = UDR0;
  TCNT0 = 0;
  TCCR0B = (1<<CS02);
}
//---------------------------------------------------------------
#pragma vector = TIMER0_COMPA_vect
__interrupt void comp_data (void)
{
   __enable_interrupt();
   UCSR0B &= ~(1<<RXCIE0);
   TCCR0B = 0;
   TCNT0 = 0;
   input_data();
   memset(rx_buff,0,sizeof(rx_buff));
   rx_counter = 0;
   UCSR0B |= (1<<RXCIE0);
}
//---------------------------------------------------------------
void USART_Transmit( unsigned char data )
{
  UDR0 = data;
  while ( !( UCSR0A & (1<<UDRE0)) );
}
//---------------------------------------------------------------
// processed input protocol message
void input_data(void)
{
   // calculate crc
   if (CRCmodbus(rx_buff,rx_counter-2) != ((rx_buff[rx_counter-2]<<8)|rx_buff[rx_counter-1]))
     return;
   
   if (rx_buff[0] != device_adress)
   {
     if(rx_buff[0] != 0xFF)return;
   }

   tmp_adress = rx_buff[0];
   
   timeout = 200;
   
   unsigned int adress =   ((rx_buff[2]<<8)|rx_buff[3]);
   unsigned int quantity = ((rx_buff[4]<<8)|rx_buff[5]);
   switch(rx_buff[1])
   {
    case 1:
           coil_register(adress,quantity);
	   break;  
     
   case 2:
           input_register(adress,quantity);
	   break;

   case 3:
	   read_holding_register(adress,quantity);
	   break;

   case 4:
           read_input_register(adress,quantity);
           break;
           
   case 5:
	   force_single_coil(adress,rx_buff[4]);
           break;
           
   case 6: // Write holding register
           write_single_register(adress,quantity);
           break;
           
   default:
	   modbus_error(ACKNOWLEDGE,rx_buff[1]);
   }
}
//---------------------------------------------------------------
// Force single coil
void force_single_coil(unsigned int adress,unsigned char status)
{
  if (status)
  {
    data_coil_register[adress/8] |= (1<<(adress & 7 ));
  }
  else
  {
   data_coil_register[adress/8] &= ~(1<<(adress & 7 ));
  }
  send_modbus_write_answer();
}
//---------------------------------------------------------------
// read holding register 
void read_holding_register(unsigned int adress,unsigned int quantity)
{
  unsigned char data[64];
  unsigned int i=0,m=0,adr;
  if(adress >= 0xFF)
  {
     adr = adress - 0xFF;
     for (i=0;i<quantity;i++)
     {
       data[m++] = (eeprom_register[adr] & 0xFF);
       data[m++] = (eeprom_register[adr] & 0xFF00) >> 8;
       adr++;
     }
  }
   else 
    memcpy(data,data_holding_register+adress,quantity*2);
  
  remix_data(data,quantity*2);
  send_modbus(0x03,quantity*2,data);
}
//---------------------------------------------------------------
// input register 0x02 Read Input register
void input_register (unsigned int adress,unsigned int quantity)
{  
     send_modbus_bit(0x02,quantity/8,data_discret_inputs);
}
//---------------------------------------------------------------
// coil register
void coil_register (unsigned int adress,unsigned int quantity)
{  
  send_modbus_bit(0x01,quantity/8,data_coil_register);
}
//---------------------------------------------------------------
// Errore Answer
void modbus_error(unsigned char error,unsigned char cmd)
{
    unsigned char data[2];
    data[0] = error;
    send_modbus(cmd | 0x80,0,data);
}
//---------------------------------------------------------------
/*
cmd - команда
data_quantity_byte - количество регистров
data - данные
*/

void send_modbus(unsigned char cmd,unsigned char data_quantity_byte,unsigned char *data)
{
	unsigned int crc = 0,i = 0;
        unsigned char buff[128];
	memset(buff,0,sizeof(buff));
	// Create packet
	buff[0] = tmp_adress;
	buff[1] = cmd;
  
        buff[2] = data_quantity_byte;
        
	memcpy(buff+3,data,data_quantity_byte);
    
        crc = CRCmodbus(buff,data_quantity_byte+3);  
	buff[data_quantity_byte+2+1] = ((crc & 0xFF00) >> 8);
	buff[data_quantity_byte+2+2] = crc & 0xFF;
        
        // Transmit Data PHY
        RX_TX1;
        delay_us(10);
        while ( !( UCSR0A & (1<<UDRE0)) );
	for (i = 0 ;i < data_quantity_byte+6-1;i++)
        {
          UDR0 = buff[i];
          while ( !( UCSR0A & (1<<UDRE0)) );
        }
        while ( !( UCSR0A & (1<<UDRE0)) );
        delay_us(85);
        RX_TX0;
}

void send_modbus_bit(unsigned char cmd,unsigned char data_quantity_byte,unsigned char *data)
{
	unsigned int crc = 0,i = 0;
        unsigned char buff[64];
	memset(buff,0,sizeof(buff));
	// Create packet
	buff[0] = tmp_adress;
	buff[1] = cmd;
  
        buff[2] = data_quantity_byte;
        
	memcpy(buff+3,data,data_quantity_byte);
    
        crc = CRCmodbus(buff,data_quantity_byte+3);  
	buff[data_quantity_byte+2+1] = ((crc & 0xFF00) >> 8);
	buff[data_quantity_byte+2+2] = crc & 0xFF;
        
        // Transmit Data PHY
        RX_TX1;
        delay_us(10);
        while ( !( UCSR0A & (1<<UDRE0)) );
	for (i = 0 ;i < data_quantity_byte+5;i++)
        {
          UDR0 = buff[i];
          while ( !( UCSR0A & (1<<UDRE0)) );
        }
        while ( !( UCSR0A & (1<<UDRE0)) );
        delay_us(85);
        RX_TX0;
}
//--------------------------------------------------------------
void send_modbus_write_answer()
{
  int i=0;
        RX_TX1;
        delay_us(10);
        while ( !( UCSR0A & (1<<UDRE0)) );
	for (i = 0 ;i < 9-1;i++)
        {
          UDR0 = rx_buff[i];
          while ( !( UCSR0A & (1<<UDRE0)) );
        }
        while ( !( UCSR0A & (1<<UDRE0)) );
        delay_us(85);
        RX_TX0;
}

void write_single_register(unsigned int adress,unsigned int data)
{
  unsigned int adr;
  if(adress >= 0xFF)
  {
    adr = adress - 0xFF;
    eeprom_register[adr] = data;
  }
  else  
    data_holding_register[adress] = data;
  send_modbus_write_answer();
  modbus_init();
  //start_state = (EEPROM_read(0x10)<<8)|EEPROM_read(0x11); TODO: Стартовое состояние выходов
}

void read_input_register(unsigned int adress,unsigned int quantity)
{
  unsigned char data[64];
  memcpy(data,data_input_register+adress,quantity*2);
  remix_data(data,quantity*2);
  send_modbus(0x04,quantity*2,data);
}

/*
 * modbus.h
 *
 *  Created on: 08.04.2012
 *      Author: TCom
 */

#ifndef MODBUS_H_
#define MODBUS_H_

#define ILLEGAL_FUNCTION          0x01
#define ILLEGAL_DATA_ADDRESS      0x02
#define ILLEGAL_DATA_VALUE        0x03
#define SLAVE_DEVICE_FAILURE      0x04
#define ACKNOWLEDGE               0x05
#define SLAVE_DEVICE_BUSY         0x06
#define NEGATIVE_ACKNOWLEDGE      0x07
#define MEMORY_PARITY_ERROR       0x08

#define READ_OUTPUT_STATUS        0x01
#define READ_INPUT_STATUS         0x02
#define READ_HOLDING_REGISTERS    0x03
#define READ_INPIT_REGISTERS      0x04
#define FORCE_SINGLE_COILS        0x05
#define PRESET_SINGLE_REGISTER    0x06
#define READ_EXEPTION_STATUS      0x07
#define READ_TIME_REGISTER        0x49
#define SET_TIME                  0x50
#define PRESET_MULTIPLE_REGISTERS 0x10
#define FORCE_MULTIPLE_COILS      0x0F

void modbus_init(void);
void input_data(void);
void read_holding_register (unsigned int adress,unsigned int quantity);
void send_modbus(unsigned char cmd,unsigned char data_quantity_byte,unsigned char *data);
void modbus_error(unsigned char error,unsigned char cmd);
void force_single_coil(unsigned int adress,unsigned char status);
void coil_register (unsigned int adress,unsigned int quantity);
void input_register(unsigned int adress,unsigned int quantity);
void write_single_register(unsigned int adress,unsigned int quantity);
void read_input_register(unsigned int adress,unsigned int quantity);
void send_modbus_write_answer();


 extern unsigned char data_coil_register[8];                     // CMD = 0x01
 extern unsigned char data_discret_inputs[8];           // CMD = 0x02
 extern unsigned int  data_holding_register[32]; // CMD = 0x03
 extern  unsigned int  data_input_register[32];          // CMD = 0x04
/*
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
*/
#endif /* MODBUS_H_ */

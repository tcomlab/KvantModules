/*
 * AD7792.h
 *
 *  Created on: 30.10.2012
 *      Author: TCom
 */

#ifndef AD7792_H_
#define AD7792_H_

void ADC_Init(void);
void ADC_configure_chanel(unsigned char Ch);
void ADC_Calibrate(void);
void ADC_Start_Continuous_Conversion_Mode(void);
int ADC_Read_Data(void);


#endif /* AD7792_H_ */

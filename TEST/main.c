// 

#include "main.h"



void main(void)
{
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
  
  OUTPUT1_1;
  OUTPUT2_1;
  OUTPUT3_1;
  OUTPUT4_1;
  OUTPUT5_1;
  OUTPUT6_1;
  OUTPUT7_1;
  OUTPUT8_1;
  OUTPUT9_1;
  OUTPUT10_1;
  OUTPUT11_1;
  OUTPUT12_1;
  OUTPUT13_1;
  OUTPUT14_1;
  
  delay_s(10);
  
  OUTPUT1_0;
  OUTPUT2_0;
  OUTPUT3_0;
  OUTPUT4_0;
  OUTPUT5_0;
  OUTPUT6_0;
  OUTPUT7_0;
  OUTPUT8_0;
  OUTPUT9_0;
  OUTPUT10_0;
  OUTPUT11_0;
  OUTPUT12_0;
  OUTPUT13_0;
  OUTPUT14_0;
  
 while(1); 
}
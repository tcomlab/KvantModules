// Blink Lib

#include "blink.h"
#include "LIB_SH\main.h"

int timeout = 5;

void blink_init(void)
{
	OCR1A = 800;
	TCCR1B =  (1<<WGM12);
	TCCR1B |= (1<<CS12)|(1<<CS10);
        TIMSK1 = (1<<OCIE1A);
}

#pragma vector = TIMER1_COMPA_vect
__interrupt void timer_compare (void)
{
  __enable_interrupt();
  static unsigned char togle = 0,time = 10;
  if (time-- == 0)
  {
  if(togle)
  {
    LED1;
    togle = 0;
  }
  else
  {
    LED0;
    togle=1;
  }
  time =10;
  }
  
  if (timeout == 0) OCR1A = 900; else {OCR1A = 200;timeout--;}
}

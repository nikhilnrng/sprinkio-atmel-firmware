#include "sprinkio.h"

void init_moisture_sensor() {
  ADMUX&=0x00; //Clearing all ADMUX bits to configure the ones we want
  ADMUX|=(1<<REFS0);//Configuring REF bit for using AVCC
  
  ADCSRA&=0b01000000; //Clearing ADCSRA bits ADEN, ADATE, ADIF, ADIE, and ADPS[2:0]
  ADCSRA|=(1<<ADEN); //Configuring ADEN to enable ADC Module
  ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//Configuring ADPS[2:0] setting the prescalar to 128
  
  DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB7);
  PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB7));
}




#include "../../sprinkio.h"

system_t sys;
settings_t settings;

int main() {
  usart_init();
  init_moisture_sensor();
  sei();
  
  while (1) {
    unsigned short value = ADC;
    ADCSRA |= (1<<ADSC);
    while ( (ADCSRA|(1<<ADSC))==ADCSRA){  // wait for conversion complete
      value = ADC ; // Get converted value
    }

    char str[10];
    sprintf(str, "%d", value);

    write_rs232_s("Sensor Output: ");
    write_rs232_s(str);
    write_rs232_s("\r\n");
  }

  return 0;
}


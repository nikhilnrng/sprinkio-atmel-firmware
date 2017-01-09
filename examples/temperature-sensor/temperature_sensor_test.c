#include "../../sprinkio.h"

system_t sys;
settings_t settings;

int main(){


  char temp_str[10];
  char humidity_str[10];
  char error[]="Read error";
  uint16_t temp;
  uint16_t humidity;
  uint8_t check_sum;
  uint8_t msb_temp;
  uint8_t lsb_temp;
  uint8_t msb_humid;
  uint8_t lsb_humid;
  uint8_t sum;
  
  PORTC=0;
  
  usart_init();
  sei();
  
  while(1){
    _delay_ms(2000);
    read_temp(&temp,&humidity,&check_sum);
    
    msb_temp=temp>>8;
    lsb_temp=temp&0x00ff;
    msb_humid=humidity>>8;
    lsb_humid=humidity&0x00ff;
    
    sum=msb_temp+lsb_temp+msb_humid+lsb_humid;
    if(sum!=check_sum){
      write_rs232_s(error);
    }
    else{
      temp/=10;
      humidity/=10;
      sprintf(temp_str, "%d C", temp);
      sprintf(humidity_str, "%d%%", humidity);
      write_rs232_s(temp_str);
      write_rs232_s(humidity_str);
    }
  }
  
  return 0;
}
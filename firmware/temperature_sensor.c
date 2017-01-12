#include "sprinkio.h"

void read_temp(uint16_t* temp, uint16_t* humidity, uint8_t* check_sum){
  char bit;
  *temp=0;
  *humidity=0;
  *check_sum=0;
  char error[]="Stuck in while loop";
  

  
  //Wait for data to be sent
  
  DDRD |= 1 << DDD2;  //Output mode
  
  //Host pulls low for 5ms
  PORTD&= ~(1<<PD2);
  _delay_ms(1);
  
  //Host pulls high for 40us
  PORTD|= 1<<PD2;
  _delay_us(40);
  
  DDRD &= ~(1 << DDD2); //Input mode

  get_pulse();
  
  int i;
  for(i=0;i<40;i++){
    bit=get_pulse();
    if(bit==-1){
      write_rs232_s(error);
      break;
    }
    if(i<16){
      *humidity|=bit<<(15-i);
    }
    else if(i<32){
      *temp|=bit<<(31-i);
    }
    else{
      *check_sum|=bit<<(39-i);
    }
  }
}

int get_pulse(){
  unsigned char count=0;
  
  //Wait until pulse goes high
  while((PIND & (1<<PD2))==0){}
  
  //Read data
  while((PIND & (1<<PD2))!=0){
    if(count==255){
      return -1;
    }
    //_delay_us(1);
    count++;
  }
  if(count>35){
    return 1;
  }
  return 0;
}
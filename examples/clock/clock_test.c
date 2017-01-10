#include "../../sprinkio.h"

system_t sys;
settings_t settings;

int main (void) {

  sei();
  i2c_init();
  usart_init();

  // set time
  time_t time;
  time.second = (uint8_t) 0;
  time.minute = (uint8_t) 55;
  time.hour = (uint8_t) 8;
  set_time(&time);

  // set date
  date_t date;
  date.day = (uint8_t) 26;
  date.month = (uint8_t) 4;
  date.year = (uint8_t) 16;
  set_date(&date);

  // current time holder
  time_t current_time;

  // current date holder
  date_t current_date;

  uint8_t i;

  for (i = 0; i < 5; i++) {
    // update current time
    get_time(&current_time);

    // update current date
    get_date(&current_date);

    // print date and time
    write_rs232_s("Date: ");
    write_rs232_date(&current_date);
    write_rs232_s(" Time: ");
    write_rs232_time(&current_time);
    write_rs232_s("\r\n");

    // delay 1 second
    _delay_ms(1000);
  }

  time_t new_time;
  add_minutes(&current_time, i, &new_time);

  write_rs232_s("New Time: ");
  write_rs232_time(&new_time);
  write_rs232_s("\r\n");

  bool comparison = compare_time(&new_time, &time);
  if (comparison) {
    write_rs232_s("Time comparison correct...\r\n");
  } else {
    write_rs232_s("Time comparison wrong...\r\n");
  }

  return 0;
}

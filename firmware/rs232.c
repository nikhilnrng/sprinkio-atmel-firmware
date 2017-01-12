#include "sprinkio.h"

/**
 * Write a byte to rs232 over serial connection
 */
void write_rs232(uint8_t data) {
  USART_SELECT_EN();
  RS232_COMM_EN();
  usart_write(data);
}

/**
 * Write a string to rs232 over serial connection
 */
void write_rs232_s(const char *s) {
  uint16_t indx = 0;
  // add string to buffer
  while (indx < strlen((char *) s)) {
    write_rs232((uint8_t) s[indx++]);
  }
  // wait for message to send
  while (!is_write_buf_empty()) {
  }
}

/**
 * Print the binary digits of a byte
 */
void write_rs232_byte(uint8_t b) {
  char str[9];
  int i;
  for (i = 0; i < 8; i++) {
      sprintf(&str[i], "%d", !!((b << i) & 0x80));
  }
  write_rs232_s(str);
}

/**
 * Write an integer to rs232 over serial connection
 */
void write_rs232_int(uint8_t num) {
  char str[10];
  sprintf(str, "%d", num);
  write_rs232_s(str);
}

/**
 * Print time from time structure pointer
 */
void write_rs232_time(time_t *t) {
  write_rs232_int(t->hour);
  write_rs232_s(":");
  write_rs232_int(t->minute);
  write_rs232_s(":");
  write_rs232_int(t->second);
}

/**
 * Print date from date structure pointer
 */
void write_rs232_date(date_t *d) {
  write_rs232_int(d->month);
  write_rs232_s("/");
  write_rs232_int(d->day);
  write_rs232_s("/");
  write_rs232_int(d->year);
}

/**
 * Print zone from zone structure pointer
 */
void write_rs232_zone(zone_t *z) {
  write_rs232_s("Zone ");
  write_rs232_byte(z->num);
  write_rs232_s("\r\n");
  write_rs232_s("Days of Week: ");
  write_rs232_byte(z->days_of_week);
  write_rs232_s("\r\n");
  write_rs232_s(" Duration: ");
  write_rs232_int(z->duration);
  write_rs232_s("\r\n");
  write_rs232_s("Moisture sensor: ");
  write_rs232_int(z->moisture_sensor);
  write_rs232_s("\r\n");
  write_rs232_s("Start Time: ");
  write_rs232_time(&(z->start_time));
  write_rs232_s("End Time: ");
  write_rs232_time(&(z->end_time));
  write_rs232_s("\r\n");
}

void write_rs232_settings(settings_t *s) {
  write_rs232_s("System Settings: \r\n");
  int i;
  for (i = 0; i < 8; i++) {
    write_rs232_zone(&(s->zones[i]));
    write_rs232_s("\r\n");
  }
  write_rs232_s("Water Saver Mode: ");
  write_rs232_int(s->mode);
  write_rs232_s("\r\n");
}

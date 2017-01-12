#ifndef SYSTEM_H
#define SYSTEM_H

#include "sprinkio.h"

// system states
#define STATE_IDLE BIT(0)
#define STATE_SETUP BIT(1)
#define STATE_ZONE BIT(2)
#define STATE_CHECK BIT(3)

// USART command characters
#define CMD_SETUP_MODE '~'
#define CMD_SENSOR_STATUS '?'
#define CMD_UPDATE_ZONE '!'
#define CMD_NO_ACTION 0xFF

void system_init(void);
void run_system_loop(void);
void system_idle_loop(void);
void system_setup_loop(void);
void system_zone_loop(void);
void system_check_loop(void);
void system_read(void);

#endif

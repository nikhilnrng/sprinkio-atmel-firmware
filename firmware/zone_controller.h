#ifndef ZONE_CONTROLLER_H
#define ZONE_CONTROLLER_H

#include "sprinkio.h"

#define MAX_ZONES 8 // max number of zones
#define MAX_DURATION 256 // max watering duration

// zone bit fields
#define ZONE_1_BF BIT(0) // 00000001
#define ZONE_2_BF BIT(1) // 00000010
#define ZONE_3_BF BIT(2) // 00000100
#define ZONE_4_BF BIT(3) // 00001000
#define ZONE_5_BF BIT(4) // 00010000
#define ZONE_6_BF BIT(5) // 00100000
#define ZONE_7_BF BIT(6) // 01000000
#define ZONE_8_BF BIT(7) // 10000000
#define ZONE_ALL_BF BIT_MASK(8) // 11111111
#define ZONE_NONE_BF 0

// USART zone run codes
#define CMD_RUN_ZONE_1 0x01
#define CMD_RUN_ZONE_2 0x02
#define CMD_RUN_ZONE_3 0x03
#define CMD_RUN_ZONE_4 0x04
#define CMD_RUN_ZONE_5 0x05
#define CMD_RUN_ZONE_6 0x06
#define CMD_RUN_ZONE_7 0x07
#define CMD_RUN_ZONE_8 0x08

// USART zone stop codes
#define CMD_STOP_ZONE_1 0x11
#define CMD_STOP_ZONE_2 0x12
#define CMD_STOP_ZONE_3 0x13
#define CMD_STOP_ZONE_4 0x14
#define CMD_STOP_ZONE_5 0x15
#define CMD_STOP_ZONE_6 0x16
#define CMD_STOP_ZONE_7 0x17
#define CMD_STOP_ZONE_8 0x18

void zone_controller_init();
bool store_zone(zone_t *zone);
void update_zones();
void run_zones(uint8_t zones);
void run_zones_manual();
void run_all_zones();
void stop_all_zones();
uint8_t read_zone_cmd();

#endif

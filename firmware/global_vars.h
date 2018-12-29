#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <util/delay.h>

#include "typedefs.h"

/** Keyboard dimensions **/
#define ROWS 5
#define COLS 14

/** Emergency reset **/
//#define ENABLE_EMERGENCY_RESET

/** Key chatter delay **/
#define KEY_CHATTER_DELAY_MS 4

extern uint8_t current_layer;
extern uint8_t next_layer;
extern const key (*current_map)[ROWS][COLS];
extern uint8_t stopped;
extern uint8_t mouse_speed;
extern int8_t pshift;

extern uint16_t row[ROWS];
extern uint16_t prev_row[ROWS];
extern uint16_t row_changes[ROWS];

#endif

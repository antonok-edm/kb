#include "global_vars.h"

enum layer current_layer = COLEMAK_l;
enum layer next_layer = COLEMAK_l;
const key (*current_map)[ROWS][COLS];
//Marker for the keyboard to pause functionality after a layer toggle
//has been released, but other keys are still held.
uint8_t stopped = 0;

//Mouse speed is translated to pixels by:
// 1/3 * 3^(mouse_speed)
uint8_t mouse_speed = 0;

//MIDI pitch shift - specifies semitonal offset for every MIDI key.
int8_t pshift = 0;

//These are used in the recognition and processing of pressed keys.
//Each matrix variable consists of a uint16_t for each row, where each,
//key is represented by one bit of the number.
uint16_t row[ROWS];
uint16_t prev_row[ROWS] = {0, 0, 0, 0, 0};
uint16_t row_changes[ROWS];

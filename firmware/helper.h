#ifndef HELPER_H
#define HELPER_H

#include <avr/wdt.h>
#include <avr/power.h>
#include <stdint.h>

#include "typedefs.h"
#include "global_vars.h"
#include "key_callable_functions.h"

#ifdef ENABLE_EMERGENCY_RESET
void debugReset(void);
#endif

void checkKeyPresses(void);
void updateKeyChanges(void);
bool key_pressed(uint8_t row_num, uint8_t column);
bool key_changed(uint8_t row_num, uint8_t column);
bool no_keys_pressed(void);
void SetupHardware(void);
void checkRow(uint16_t* row, uint8_t pin);

void doKeyFunctionCalls(void);
void doKeyMIDItasks(void);
void read_from_mapping_FUNC(const key key_mapping[ROWS][COLS]);
void read_from_mapping_HID(uint8_t* Modifier,
                           uint8_t* KeyCodes,
                           uint8_t* UsedKeyCodes,
                           const key key_mapping[ROWS][COLS]);
void read_from_mapping_MIDI(const key key_mapping[ROWS][COLS]);
void read_from_mapping_MOUSE(uint8_t* Button,
                             int8_t* Mouse_X,
                             int8_t* Mouse_Y,
                             int8_t* Scroll_X,
                             int8_t* Scroll_Y,
                             const key key_mapping[ROWS][COLS]);
void checkRolloverError(uint8_t* KeyCode,
                        uint8_t* UsedKeyCodes,
                        uint8_t max_keycodes);
extern void sendMIDI(uint8_t MIDICommand,
                     uint8_t Channel,
                     uint8_t MIDIPitch);

#endif

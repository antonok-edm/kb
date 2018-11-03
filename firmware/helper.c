#include "helper.h"

#ifdef ENABLE_EMERGENCY_RESET
//Turn this on before programming any functionality that could break
//the keyboard's processing loop or function calling abilities. When
//enabled, this function is called as the keyboard is powered on,
//and the bootloader can be enabled by holding the upper right button.
void debugReset(void) {
    PORTD |= (1<<0);
    _delay_ms(500);
    if (bit_is_set(PINB,0)) resetToBootloader(0);
    PORTD&= (0<<0);
}
#endif

//Update the row mask for each row of keys on the keyboard.
void checkKeyPresses(void) {
    checkRow(&row[0], 0);
    checkRow(&row[1], 1);
    checkRow(&row[2], 2);
    checkRow(&row[3], 3);
    checkRow(&row[4], 5);
}

//Update the row_changes mask for each row of keys on the keyboard,
//and store the previous state of the keyboard for the next loop.
void updateKeyChanges(void) {
    for(uint8_t n = 0; n < ROWS; n++)
            row_changes[n] = row[n] ^ prev_row[n];
    for(uint8_t n = 0; n < ROWS; n++)
            prev_row[n] = row[n];
}

//Helper function to check if a specific key is pressed.
bool key_pressed(uint8_t row_num, uint8_t column) {
    return (row[row_num] & (1 << column));
}

//Helper function to check if a specific key's state has just changed.
bool key_changed(uint8_t row_num, uint8_t column) {
    return (row_changes[row_num] & (1 << column));
}

//Helper function to check if all keys are released.
bool no_keys_pressed(void) {
    for (int i = 0; i < ROWS; i++) {
            if(row[i]) return false;
    }
    return true;
}

// Configures the board hardware and chip peripherals.
void SetupHardware(void) {
    //Disable watchdog if enabled by bootloader/fuses
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
    //Disable clock division
    clock_prescale_set(clock_div_1);
    //Initialize the LUFA USB device.
    USB_Init();
}

// Outputs a signal to the specified row and updates that
// row's mask variable if a signal is returned through a column.
void checkRow(uint16_t* row, uint8_t pin) {
    *row = 0;
    PORTD |= (1 << pin);
    //Configure delay in global_vars.h
    _delay_ms(KEY_CHATTER_DELAY_MS);
    if (bit_is_set(PINF,7)) *row |= (1 << 0);
    if (bit_is_set(PINF,6)) *row |= (1 << 1);
    if (bit_is_set(PINF,5)) *row |= (1 << 2);
    if (bit_is_set(PINF,4)) *row |= (1 << 3);
    if (bit_is_set(PINF,1)) *row |= (1 << 4);
    if (bit_is_set(PINF,0)) *row |= (1 << 5);
    if (bit_is_set(PINB,7)) *row |= (1 << 6);
    if (bit_is_set(PINB,6)) *row |= (1 << 7);
    if (bit_is_set(PINB,5)) *row |= (1 << 8);
    if (bit_is_set(PINB,4)) *row |= (1 << 9);
    if (bit_is_set(PINB,3)) *row |= (1 << 10);
    if (bit_is_set(PINB,2)) *row |= (1 << 11);
    if (bit_is_set(PINB,1)) *row |= (1 << 12);
    if (bit_is_set(PINB,0)) *row |= (1 << 13);
    PORTD &= (0 << pin);
}

void doKeyFunctionCalls(void) {
    if(!stopped) read_from_mapping_FUNC(*current_map);
}

void doKeyMIDItasks(void) {
    if(!stopped) read_from_mapping_MIDI(*current_map);
}

// Call any functions pointed to by a key that was just pressed or released.
void read_from_mapping_FUNC(const key key_mapping[ROWS][COLS]) {
    for(uint8_t i = 0; i < ROWS; i++) {
            for(uint8_t j = 0; j < COLS; j++) {
                    if(key_changed(i, j) && key_mapping[i][j].type == FUNC_t)
                            ((void (*)(uint8_t))key_mapping[i][j].data)(key_pressed(i,j));
            }
    }
}

//Write the keycodes of all pressed HID and MOD keys in the specified keymap
//to an array of keycodes to be reported to the host.
void read_from_mapping_HID(uint8_t* Modifier,
                           uint8_t* KeyCodes,
                           uint8_t* UsedKeyCodes,
                           const key key_mapping[ROWS][COLS]) {
    for(uint8_t i = 0; i < ROWS; i++) {
            for(uint8_t j = 0; j < COLS; j++) {
                    if(key_pressed(i, j)) {
                            if(key_mapping[i][j].type == HID_t)
                                    KeyCodes[(*UsedKeyCodes)++] = (uint8_t) key_mapping[i][j].data;
                            else if(key_mapping[i][j].type == MOD_t)
                                    *Modifier |= (uint8_t) key_mapping[i][j].data;
                            else if(key_mapping[i][j].type == HIDMOD_t) {
                                    KeyCodes[(*UsedKeyCodes)++] = (uint8_t) (key_mapping[i][j].data >> 8);
                                    *Modifier |= (uint8_t) key_mapping[i][j].data;
                            }
                    }
            }
    }
}

//Send all pressed MIDI keys in the specified keymap in a report to the host.
void read_from_mapping_MIDI(const key key_mapping[ROWS][COLS]) {
    uint8_t MIDICommand = 0;
    uint8_t MIDIPitch;
    uint8_t Channel = MIDI_CHANNEL(1);   //Only one MIDI channel for now.

    for(uint8_t i = 0; i < ROWS; i++) {
        for(uint8_t j = 0; j < COLS; j++) {
            if(key_changed(i, j)) {
                if(key_mapping[i][j].type == MIDI_t) {
                    MIDIPitch = (int8_t) key_mapping[i][j].data + pshift;
                    if (key_pressed(i, j)) MIDICommand = MIDI_COMMAND_NOTE_ON;
                    else MIDICommand = MIDI_COMMAND_NOTE_OFF;
                    sendMIDI(MIDICommand, Channel, MIDIPitch);
                }
                //TODO: Add other MIDI capabilities such as emulated
                //faders and potentiometers, and arbitrary MIDI-
                //mappable control buttons.
            }
        }
    }
}

//Send commands from all pressed CLICK, MOUSE_X, MOUSE_Y, SCROLL_X, and SCROLL_Y
//keys in the specified keymap to the host as mouse movements.
void read_from_mapping_MOUSE(uint8_t* Button,
                             int8_t* Mouse_X,
                             int8_t* Mouse_Y,
                             int8_t* Scroll_X,
                             int8_t* Scroll_Y,
                             const key key_mapping[ROWS][COLS]) {
    for(uint8_t i = 0; i < ROWS; i++) {
        for(uint8_t j = 0; j < COLS; j++) {
            if(key_pressed(i, j)) {
                if(key_mapping[i][j].type == CLICK_t) {
                    *Button |= (1 << (uint8_t) key_mapping[i][j].data);
                }
                else if(key_mapping[i][j].type == MOUSE_X_t) {
                    *Mouse_X += (int8_t) key_mapping[i][j].data;
                }
                else if(key_mapping[i][j].type == MOUSE_Y_t) {
                    *Mouse_Y += (int8_t) key_mapping[i][j].data;
                }
                else if(key_mapping[i][j].type == SCROLL_X_t) {
                    *Scroll_X += (int8_t) key_mapping[i][j].data;
                }
                else if(key_mapping[i][j].type == SCROLL_Y_t) {
                    *Scroll_Y += (int8_t) key_mapping[i][j].data;
                }
            }
        }
    }
}

void checkRolloverError(uint8_t* KeyCode, uint8_t* UsedKeyCodes, uint8_t max_keycodes) {
    if (*UsedKeyCodes >= max_keycodes) {
        for(int i = 0; i < 14; i++) {
            KeyCode[i] = 0;
        }
        KeyCode[0] = HID_KEYBOARD_SC_ERROR_ROLLOVER;
    }
}

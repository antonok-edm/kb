#include "key_callable_functions.h"

//Used when updating firmware. To reset without
//reprogramming, it is sufficient to unplug and
//replug the keyboard.
//This code safely disables USB and then
//activates the Teensy Halfkay Bootloader.
void resetToBootloader(uint8_t pressed) {
        USB_Detach();
        _delay_ms(5);
        asm volatile("jmp 0x7E00");
}

//Compile-time function generator for layer toggle functions
#define TOGGLE_FUNCTION(functionName, layer)\
        void functionName(uint8_t pressed) {\
                if(pressed) {\
                        next_layer = current_layer;\
                        current_layer = layer;\
                }\
                else {\
                        current_layer = next_layer;\
                        stopped = 1;\
                }\
        }

//Compile-time function generator for layer target functions
#define TARGET_FUNCTION(functionName, layer)\
        void functionName(uint8_t pressed) {\
                if(pressed) {\
                        next_layer = layer;\
                }\
        }

//Layer toggle functions. Create one for each layer that
//should be active while holding a modifier key.
TOGGLE_FUNCTION(toggleLayer_LRDN, LRDN_l)
TOGGLE_FUNCTION(toggleLayer_LRUP, LRUP_l)
TOGGLE_FUNCTION(toggleLayer_FN, FN_l)
TOGGLE_FUNCTION(toggleLayer_MIDI, MIDI_l)
TOGGLE_FUNCTION(toggleLayer_QWERTY, QWERTY_l)
TOGGLE_FUNCTION(toggleLayer_COLEMAK, COLEMAK_l)
TOGGLE_FUNCTION(toggleLayer_MOUSE, MOUSE_l)

//Layer target functions. Create one for each layer that
//should be active without holding a modifier key.
TARGET_FUNCTION(targetLayer_COLEMAK, COLEMAK_l)
TARGET_FUNCTION(targetLayer_QWERTY, QWERTY_l)
TARGET_FUNCTION(targetLayer_MIDI, MIDI_l)

//MIDI pitch-shift keys
void increasePitch(uint8_t pressed) {
        if(pressed) pshift++;
}
void decreasePitch(uint8_t pressed) {
        if(pressed) pshift--;
}
void resetPitch(uint8_t pressed) {
        if(pressed) pshift = 0;
}

//Mouse speed toggle keys
void mouseSpeed_0(uint8_t pressed) {
        if(pressed) mouse_speed = 0;
}
void mouseSpeed_1(uint8_t pressed) {
        if(pressed) mouse_speed = 1;
}
void mouseSpeed_2(uint8_t pressed) {
        if(pressed) mouse_speed = 2;
}
void mouseSpeed_3(uint8_t pressed) {
        if(pressed) mouse_speed = 3;
}

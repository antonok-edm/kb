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

//Compile-time function generator for layer toggle functions.
//Used to access layers while holding a modifier key.
#define TOGGLE_FUNCTION(layer)\
    void toggleLayer_ ## layer(uint8_t pressed) {\
        if(pressed) {\
            next_layer = current_layer;\
            current_layer = layer ## _l;\
        }\
        else {\
            current_layer = next_layer;\
            stopped = 1;\
        }\
    }

//Compile-time function generator for layer target functions.
//Used to access layers that can be active without holding a
//modifier key.
#define TARGET_FUNCTION(layer)\
    void targetLayer_ ## layer(uint8_t pressed) {\
        if(pressed) {\
            next_layer = layer ## _l;\
        }\
    }

//Compile-time function generator for both layer target and
//layer toggle functions
#define LAYER_FUNCTIONS(layer)\
    TOGGLE_FUNCTION(layer)\
    TARGET_FUNCTION(layer)

//Add lines here for each defined layer
LAYER_FUNCTIONS(QWERTY)
LAYER_FUNCTIONS(COLEMAK)
LAYER_FUNCTIONS(FN)
LAYER_FUNCTIONS(LRDN)
LAYER_FUNCTIONS(LRUP)
LAYER_FUNCTIONS(MIDI)
LAYER_FUNCTIONS(MOUSE)

//Compile-time function generator for mouse speed functions.
//Used to toggle the speed of the cursor when using mouse
//movement keys
#define MOUSESPEED(speed)\
    void mouseSpeed_ ## speed(uint8_t pressed) {\
        if(pressed) mouse_speed = speed;\
    }

//Add lines here for each desired mouse speed
MOUSESPEED(0)
MOUSESPEED(1)
MOUSESPEED(2)
MOUSESPEED(3)

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

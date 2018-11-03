#ifndef TYPEDEFS_H
#define TYPEDEFS_H

enum layer {
    COLEMAK_l,
    QWERTY_l,
    FN_l,
    LRUP_l,
    LRDN_l,
    MIDI_l,
    MOUSE_l
};

enum key_type {
    NONE_t,
    HID_t,
    MOD_t,
    MIDI_t,
    FUNC_t,
    CLICK_t,
    MOUSE_X_t,
    MOUSE_Y_t,
    SCROLL_X_t,
    SCROLL_Y_t,
    HIDMOD_t
};

typedef struct key {
    enum key_type type;
    uint16_t data;
} key;

#endif

#ifndef LAYERMAPS_H
#define LAYERMAPS_H

#define NONE                            (key){NONE_t, 0}
#define HID(code)                       (key){HID_t, code}
#define MOD(code)                       (key){MOD_t, code}
#define MIDI(pitch)                     (key){MIDI_t, pitch}
#define FUNC(fn)                        (key){FUNC_t, (uint16_t) &fn}
#define CLICK(button)                   (key){CLICK_t, button}
#define MOUSE_X(amount)                 (key){MOUSE_X_t, (uint16_t) amount}
#define MOUSE_Y(amount)                 (key){MOUSE_Y_t, (uint16_t) amount}
#define SCROLL_X(amount)                (key){SCROLL_X_t, (uint16_t) amount}
#define SCROLL_Y(amount)                (key){SCROLL_Y_t, (uint16_t) amount}
#define HIDMOD(hidcode, modcode)        (key){HIDMOD_t, ((uint16_t) hidcode << 8) + (modcode)}

#define KEYMAP(layer) const key layer ## _map[ROWS][COLS] = 

#define TOGGLE(layer) FUNC(toggleLayer_ ## layer)
#define TARGET(layer) FUNC(targetLayer_ ## layer)

//Layermaps.c is included inline because it provides global constant array definitions.
//Using forward declarations would add another step to configuration.
#include "layermaps/layermaps.c"

#undef NONE
#undef HID
#undef MOD
#undef MIDI
#undef FUNC
#undef CLICK
#undef MOUSE_X
#undef MOUSE_Y
#undef SCROLL_X
#undef SCROLL_Y
#undef HIDMOD

#undef KEYMAP

#undef TOGGLE
#undef TARGET

#endif

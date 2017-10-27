# kb
This repository contains the full design files for my custom-designed keyboard.

Features:
- 68 keys
- compact ortholinear format
- carbon fiber construction
- full on-the-fly programmability
- HID Mouse functionality
- MIDI functionality
- Built-in Colemak layout
- 14-key HID rollover
- n-key MIDI rollover
- multiple function layers

## My setup
I currently have 7 different layers defined on my keyboard: Colemak, Qwerty, MIDI, Fn, Layer-Up, Layer-Down, and Mouse.

MIDI has a 4x12 grid of chromatic MIDI keys, providing 4 octaves of pitch-shiftable notes.

Fn has all standard function keys, as well as keyboard-internal functions like switching between layers.

Layer-Down puts symbols for programming, such as parenthesis and braces, on the home row so that they can be accessed more easily.

Layer-Up has a numpad.

Mouse puts cursor controls on the right side of the keyboard, with cursor speed controls operated by the left hand.

## Customizing the keyboard
My firmware allows for easy remapping of keys on any layer. `layermaps.h` includes the behavior of every key. Each key can take one of the following functions:

| Function | Description                                                                     | Examples                                |
|----------|---------------------------------------------------------------------------------|-----------------------------------------|
| HID      | Any standard keyboard key                                                       | a, 1, ;                                 |
| MOD      | At least one standard keyboard modifier                                         | Left shift, Right control               |
| MIDI     | Any MIDI pitch                                                                  | C0 to G10                               |
| FUNC     | Any arbitrary predefined function from the firmware                             | shift MIDI pitch, switch function layer |
| CLICK    | Execute a click an HID mouse button                                             | left click, middle click, forward/back  |
| MOUSE    | Move the mouse cursor                                                           | horizontal or vertical                  |
| SCROLL   | Mouse scroll or pan                                                             | horizontal or vertical                  |
| HIDMOD   | Any standard keyboard key combined with at least one standard keyboard modifier | ctrl+shift+m, {, ?                      |

Once modifications have been made to a layout, the firmware can be reflashed to the keyboard using the standard Teensy programmer over USB. The Teensy's bootloader
can be accessed without taking apart the keyboard by pressing a predefined key combination. The entire reflashing process takes about 3 seconds in total.

## Want to use this for your own keyboard?
If this project is useful at all for you, I'd love to hear about it!

The firmware is optimized for my own personal setup, but could be tweaked with minimal effort to suit different hardware. Please feel free to submit an issue, and I would
be happy to provide assistance with this.

### License information
Files from the LUFA library are released under the MIT license.
My custom design files are released under version 3 or later of the GPL.
License information for the LUFA library is available at www.lufa-lib.org.

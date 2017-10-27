#ifndef KEY_CALLABLE_FUNCTIONS_H
#define KEY_CALLABLE_FUNCTIONS_H

#include <stdint.h>

#include "global_vars.h"

#include <LUFA/Drivers/USB/USB.h>

void resetToBootloader(uint8_t pressed);

void toggleLayer_LRDN(uint8_t pressed);
void toggleLayer_LRUP(uint8_t pressed);
void toggleLayer_FN(uint8_t pressed);
void toggleLayer_MIDI(uint8_t pressed);
void toggleLayer_QWERTY(uint8_t pressed);
void toggleLayer_COLEMAK(uint8_t pressed);
void toggleLayer_MOUSE(uint8_t pressed);

void targetLayer_COLEMAK(uint8_t pressed);
void targetLayer_QWERTY(uint8_t pressed);
void targetLayer_MIDI(uint8_t pressed);

void increasePitch(uint8_t pressed);
void decreasePitch(uint8_t pressed);
void resetPitch(uint8_t pressed);

void mouseSpeed_0(uint8_t pressed);
void mouseSpeed_1(uint8_t pressed);
void mouseSpeed_2(uint8_t pressed);
void mouseSpeed_3(uint8_t pressed);

#endif

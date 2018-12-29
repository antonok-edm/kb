#ifndef KEY_CALLABLE_FUNCTIONS_H
#define KEY_CALLABLE_FUNCTIONS_H

#include <stdint.h>

#include "global_vars.h"

#include <LUFA/Drivers/USB/USB.h>

void resetToBootloader(uint8_t pressed);

void toggleLayer_0(uint8_t pressed);
void toggleLayer_1(uint8_t pressed);
void toggleLayer_2(uint8_t pressed);
void toggleLayer_3(uint8_t pressed);
void toggleLayer_4(uint8_t pressed);
void toggleLayer_5(uint8_t pressed);
void toggleLayer_6(uint8_t pressed);

void targetLayer_0(uint8_t pressed);
void targetLayer_1(uint8_t pressed);
void targetLayer_2(uint8_t pressed);
void targetLayer_3(uint8_t pressed);
void targetLayer_4(uint8_t pressed);
void targetLayer_5(uint8_t pressed);
void targetLayer_6(uint8_t pressed);

void increasePitch(uint8_t pressed);
void decreasePitch(uint8_t pressed);
void resetPitch(uint8_t pressed);

void mouseSpeed_0(uint8_t pressed);
void mouseSpeed_1(uint8_t pressed);
void mouseSpeed_2(uint8_t pressed);
void mouseSpeed_3(uint8_t pressed);

#endif

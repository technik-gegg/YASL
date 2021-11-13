#pragma once
#include "Config.h"

extern CRGBPalette16  currentPalette;
extern CRGBPalette16  targetPalette;
extern uint8_t        maxChanges;
extern TBlendType     currentBlending;

void Ripple(int speedDelay = 50);
void RipplePal(int speedDelay = 50);
int Wrap(int step);

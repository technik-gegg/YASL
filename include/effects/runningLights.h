#pragma once
#include "Config.h"

extern CRGBPalette16  currentPalette;
extern CRGBPalette16  targetPalette;

void RunningLights(byte red, byte green, byte blue, int waveDelay);

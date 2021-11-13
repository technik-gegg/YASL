#pragma once
#include "Config.h"

extern CRGBPalette16  currentPalette;
extern CRGBPalette16  targetPalette;
extern uint8_t        maxChanges;

void Noise8mover(int xscale = 30, int yscale = 30);
void FillNoise8(int xscale = 30, int yscale = 30);
void Noise16_3();

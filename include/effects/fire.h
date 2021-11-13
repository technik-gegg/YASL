#pragma once
#include "Config.h"

extern bool             gReverseDirection;
extern CRGBPalette16    gPal;
extern CRGBPalette16    firePalette;
extern CRGBPalette16    firePaletteBlue;

void Fire(int cooling, int sparking, int speedDelay, bool isLeft);
void setPixelHeatColor(int index, byte temperature);
void Fire2012WithPalette(int cooling, int sparking, bool reverseDirection);

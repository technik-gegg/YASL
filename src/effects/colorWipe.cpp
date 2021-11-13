#include "effects/colorWipe.h"

static int    wipeStep = 0;
static byte   wipeR, wipeG, wipeB;
static int    wipeSpeedDelay = 0;
static bool   wipeCycleDone;
unsigned      wipeCycle = 0;

void ColorWipe(byte red, byte green, byte blue, int speedDelay) {
  if(wipeStep == 0) {
      wipeR = red;
      wipeG = green;
      wipeB = blue;
      wipeCycleDone = false;
      wipeSpeedDelay = speedDelay;
  }
  if(wipeStep >= NUM_LEDS) {
    wipeStep = 0;
    wipeCycleDone = true;
    wipeCycle++;
  }
  else
    wipeStep++;

  setPixel(wipeStep, wipeR, wipeG, wipeB);
  showStrip();
  delay(speedDelay);
}

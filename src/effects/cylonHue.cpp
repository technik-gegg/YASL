#include "effects/CylonHue.h"

static int    cylonStep = 0;
static int    cylonDir = 0;
static int    cylonSpeedDelay = 0;
static bool   cylonCycleDone;
static uint8_t cylonHue = 0;

void CylonHue(int speedDelay) { 
  
  if(cylonDir == 0) {
    if(cylonStep == 0) {
      cylonHue = 0;
      cylonCycleDone = false;
      cylonSpeedDelay = speedDelay;
    }
    if(cylonStep >= NUM_LEDS) {
      cylonDir = 1;
    }
    cylonStep++;
  }
  else {
    cylonStep--;
    if(cylonStep == 0) {
      cylonDir = 0;
      cylonCycleDone = true;
    }
  }

  setPixel(cylonStep, CHSV(cylonHue++, 255, 255));
  showStrip();
  fadeAll();
  delay(cylonSpeedDelay);
}

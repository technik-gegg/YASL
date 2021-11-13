#include "effects/rgbLoop.h"

static int    rgbloopStep = 0;
static int    rgbloopDir = 0;
static int    rgbloopCount = 0;
static int    rgbloopSpeedDelay = 0;
static bool   rgbloopCycleDone;

void RgbLoop(int speedDelay) {

  if(rgbloopDir == 0) {
    if(rgbloopStep == 0) {
      rgbloopCycleDone = false;
      rgbloopSpeedDelay = speedDelay;
    }
    if(rgbloopStep >= NUM_LEDS) {
      rgbloopDir = 1;
    }
    rgbloopStep++;
  }
  else {
    rgbloopStep--;
    if(rgbloopStep == 0) {
      rgbloopDir = 0;
      rgbloopCycleDone = true;
      rgbloopCount++;
    }
  }
  switch(rgbloopCount % 6) { 
        case 0: setAll(rgbloopStep,0,0); break;
        case 1: setAll(0,rgbloopStep,0); break;
        case 2: setAll(0,0,rgbloopStep); break;
        case 3: setAll(rgbloopStep,rgbloopStep,0); break;
        case 4: setAll(0,rgbloopStep,rgbloopStep); break;
        case 5: setAll(rgbloopStep,0,rgbloopStep); break;
  }
  showStrip();
  delay(rgbloopSpeedDelay);
}

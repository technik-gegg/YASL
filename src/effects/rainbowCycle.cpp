#include "effects/rainbowCycle.h"


static int    rainbowStep = 0;
static int    rainbowSpeedDelay = 0;
static bool   rainbowCycleDone;

void RainbowCycle(int speedDelay) {

  if(rainbowStep == 0) {
    rainbowCycleDone = false;
    rainbowSpeedDelay = speedDelay;
  }
  if(rainbowStep < 256*5) {
    byte *color;
    for(int i=0; i< NUM_LEDS; i++) {
      color = Wheel(((i * 256 / NUM_LEDS) + rainbowStep) & 255);
      setPixel(i, *color, *(color+1), *(color+2));
    }
    showStrip();
    delay(rainbowSpeedDelay);
    rainbowStep++;
  }
  else {
    rainbowStep = 0;
    rainbowCycleDone = true;
  }
  //Serial.println(rainbowStep);
}

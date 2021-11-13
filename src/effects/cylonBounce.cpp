#include "effects/cylonBounce.h"

static int    cyclonStep = 0;
static int    cyclonDir = 0;
static byte   cyclonR, cyclonG, cyclonB;
static int    cyclonSpeedDelay = 0, cyclonReturnDelay;
static bool   cyclonCycleDone;

void CylonBounce(byte red, byte green, byte blue, int eyeSize, int speedDelay, int returnDelay) {
  float r, g, b;

  if(cyclonDir == 0) {
    if(cyclonStep == -eyeSize) {
      cyclonR = red;
      cyclonG = green;
      cyclonB = blue;
      cyclonCycleDone = false;
      cyclonSpeedDelay = speedDelay;
      cyclonReturnDelay = returnDelay;
    }
    if(cyclonStep >= NUM_LEDS - 1) {// eyeSize - 2) {
      cyclonDir = 1;
    }
    cyclonStep++;
  }
  else {
    cyclonStep--;
    if(cyclonStep == -eyeSize) {
      cyclonDir = 0;
      cyclonCycleDone = true;
    }
  }

  r = cyclonR/10;
  g = cyclonG/10;
  b = cyclonB/10;

  setAll(0,0,0);
  setPixel(cyclonStep, r, g, b);
  for(int j = 1; j <= eyeSize; j++) {
    setPixel(cyclonStep + j, cyclonR, cyclonG, cyclonB); 
  }
  setPixel(cyclonStep + eyeSize + 1, r, g, b);
  showStrip();
  delay(cyclonSpeedDelay);

  if(cyclonCycleDone)
    delay(cyclonReturnDelay);
  //Serial.println(cyclonStep);
}

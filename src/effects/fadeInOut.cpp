#include "effects/fadeInOut.h"

static int    fadeStep = 0;
static int    fadeDir = 0;
static byte   fadeR, fadeG, fadeB;
static int    fadeDelay = 0;
static bool   fadeCycleDone = false;

void FadeInOut(byte red, byte green, byte blue, int speedDelay){
  float r, g, b;

  if(fadeDir == 0) {
    if(fadeStep == 0) {
      fadeR = red;
      fadeG = green;
      fadeB = blue;
      fadeCycleDone = false;
      fadeDelay = speedDelay;
    }
    if(fadeStep >= 255) {
      fadeDir = 1;
    }
    fadeStep++;
  }
  else {
    fadeStep--;
    if(fadeStep == 0) {
      fadeDir = 0;
      fadeCycleDone = true;
    }
  }
  r = (fadeStep/256.0)*fadeR;
  g = (fadeStep/256.0)*fadeG;
  b = (fadeStep/256.0)*fadeB;
  setAll(r,g,b);
  showStrip();
  delay(fadeDelay);
  //Serial.println(fadeStep);
}

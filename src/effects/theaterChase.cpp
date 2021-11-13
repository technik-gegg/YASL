#include "effects/theaterChase.h"

static int    theaterStep = 0;
static byte   theaterR, theaterG, theaterB;
static int    theaterSpeedDelay = 0;
static bool   theaterCycleDone;

void TheaterChase(byte red, byte green, byte blue, int speedDelay) {

  if(theaterStep == 0) {
    theaterR = red;
    theaterG = green;
    theaterB = blue;
    theaterCycleDone = false;
    theaterSpeedDelay = speedDelay;
  }
  if(theaterStep < 10) {
    for (int j=0; j < 3; j++) {
      for (int i = 0; i < NUM_LEDS; i += 3) {
        setPixel(i+j, theaterR, theaterR, theaterR);
      }
      showStrip();
      delay(speedDelay);
      for (int i = 0; i < NUM_LEDS; i += 3) {
        setPixel(i+j, 0,0,0);
      }
    }
    theaterStep++;
  }
  else {
    theaterStep = 0;
    theaterCycleDone = true;
  }
}

void TheaterChaseRainbow(int speedDelay) {
  byte *color;
  
  if(theaterStep == 0) {
    theaterCycleDone = false;
    theaterSpeedDelay = speedDelay;
  }
  if(theaterStep < 256) {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < NUM_LEDS; i += 3) {
          color = Wheel( (i + theaterStep) % 255);
          setPixel(i + j, *color, *(color+1), *(color+2));
        }
        showStrip();
        delay(speedDelay);
       
        for (int i = 0; i < NUM_LEDS; i += 3) {
          setPixel(i + j, 0,0,0);
        }
    }
    theaterStep++;
  }
  else {
    theaterStep = 0;
    theaterCycleDone = true;
  }
  
}

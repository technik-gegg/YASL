#include "effects/meteorRain.h"

static int    meteorStep = 0;
static int    meteorDir = 0;
static byte   meteorR, meteorG, meteorB;
static int    meteorSpeedDelay = 0;
static bool   meteorCycleDone;
static byte   meteorSize;
static byte   meteorTrailDecay;
static bool   meteorRandomDecay;


void MeteorRain(byte red, byte green, byte blue, byte _size, byte trailDecay, bool randomDecay, int speedDelay, int dir) {  

  if(meteorStep == 0) {
    setAll(0,0,0);
    meteorR = red;
    meteorG = green;
    meteorB = blue;
    //Serial.print(red); Serial.print("-"); Serial.print(green); Serial.print("-"); Serial.println(blue);
    meteorSpeedDelay = speedDelay;
    meteorSize = _size;
    meteorTrailDecay = trailDecay;
    meteorRandomDecay = randomDecay;
    meteorCycleDone = false;
    meteorDir = dir;
    Serial.println(meteorDir%2);
  }
  
  
  if(meteorStep < NUM_LEDS*4) {
    if(meteorDir %2 == 0) {
    // fade brightness all LEDs one step
      for(int j=0; j < NUM_LEDS; j++) {
        if( (!meteorRandomDecay) || (random(10)>5) ) {
          fadeToBlack(j, meteorTrailDecay );        
        }
      }
      // draw meteor
      for(int j = 0; j < meteorSize; j++) {
        if((meteorStep - j < NUM_LEDS) && (meteorStep - j >= 0)) {
          setPixel(meteorStep - j, meteorR, meteorG, meteorB);
        } 
      }
    }
    else {
      for(int j = NUM_LEDS-1; j > 0; j--) {
        if( (!meteorRandomDecay) || (random(10)>5) ) {
          fadeToBlack(j, meteorTrailDecay );        
        }
      }
      // draw meteor
      for(int j = NUM_LEDS-1; j > meteorSize; j--) {
        //Serial.println(j-meteorStep);
        if((j - meteorStep < NUM_LEDS) && (j - meteorStep >= 0)) {
          setPixel(j - meteorStep, meteorR, meteorG, meteorB);
        } 
      }
    }
    meteorStep++;
  }
  else {
    meteorStep = 0;
    meteorCycleDone = true;
  }
  showStrip();
  delay(meteorSpeedDelay);
}

bool GetMeteorCycleDone() {
  return meteorCycleDone; 
}

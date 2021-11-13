#include "effects/twinkle.h"

static int    twinkleStep = 0;
static int    twinkleSpeedDelay = 0;
static bool   twinkleCycleDone;
CHSV          twinkleLeds[NUM_LEDS];
byte          twinkleInc[NUM_LEDS];
byte          twinkleDir[NUM_LEDS];
byte          twinkleDelay[NUM_LEDS];
bool          twinkleDone[NUM_LEDS];

void Twinkle(CHSV color, int count, int speedDelay, bool allRandom) {
  
  if(twinkleStep == 0) {
    for(int i=0; i<NUM_LEDS; i++) {
      twinkleLeds[i] = CHSV(0,0,0);
      twinkleInc[i] = 0;
      twinkleDir[i] = 0;
      twinkleDone[i] = true;
      twinkleDelay[i] = 0;
    }
    randomSeed(analogRead(0));
    for(int i=0; i<count; i++) {
      int index = random(0, NUM_LEDS-1);
      twinkleLeds[index] = allRandom ? rgb2hsv_approximate(CRGB(random8(), random8(), random8())) : color;
      twinkleLeds[index].value = 0;
      twinkleLeds[index].saturation = 255;
      twinkleInc[index] = random(2, 8);
      twinkleDone[index] = false;
      twinkleDelay[index] = random(0, 50);
    }
    /*
    for(int i=0; i<NUM_LEDS; i++) {
      if(twinkleInc[i]>0) {
        Serial.print(i); Serial.print(": "); 
        Serial.print(twinkleLeds[i].hue);Serial.print(",");Serial.print(twinkleLeds[i].saturation);Serial.print(",");Serial.print(twinkleLeds[i].value);
        Serial.print(" | "); Serial.print(twinkleInc[i]); Serial.print(" | "); Serial.println(twinkleDelay[i]); 
      }
    }
    */
    twinkleCycleDone = false;
    twinkleSpeedDelay = speedDelay;
    setAll(0,0,0);
    showStrip();
  }

  twinkleStep++;

  if(twinkleStep > 0) {
    for(int i=0; i<NUM_LEDS; i++) {
      if(twinkleInc[i] > 0 && !twinkleDone[i]) {
        if(twinkleDir[i] == 1) {
          if(twinkleLeds[i].value - twinkleInc[i] > 0)
            twinkleLeds[i].value -= twinkleInc[i];
          else {
            twinkleDone[i] = true;
            twinkleLeds[i].value = 0;
          }
        }
        else if(twinkleDir[i] == 0 && twinkleDelay[i] <= twinkleStep) {
          if(twinkleLeds[i].value + twinkleInc[i] < 255)
            twinkleLeds[i].value += twinkleInc[i];
          else {
            twinkleLeds[i].value = 255;
            twinkleDir[i] = 1;
          }
        }
        setPixel(i, twinkleLeds[i]);
      }
    }
    showStrip();
    delay(speedDelay);
  }
  twinkleCycleDone = true;
  for(int i=0; i<NUM_LEDS; i++) {
    if(twinkleDone[i] == false)
      twinkleCycleDone = false;
  }
  if(twinkleCycleDone){
    twinkleStep = 0;
  }
}

void TwinkleRandom(int count, int speedDelay) {
  Twinkle(CHSV(random8(),random8(),random8()), count, speedDelay, true);
}

#include "effects/blendwave.h"

CRGB clr1;
CRGB clr2;
uint8_t loc1;
uint8_t loc2;
uint8_t ran1;
uint8_t ran2;

void BlendWave(int speed) {    // blendwave sample from A. Tuline

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

  showStrip();
}

void DotBeat(int bpm, int fadeVal) { // dot_beat sample from A. Tuline

  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  setPixel(middle, CRGB::Purple);
  setPixel(inner, CRGB::Blue);
  setPixel(outer, CRGB::Aqua);

  nscale8(leds,NUM_LEDS,fadeVal);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
  showStrip();
}

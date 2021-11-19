#include "ledUtils.h"

CRGB  leds[NUM_LEDS];
CRGB  leds2[NUM_LEDS2];
byte  shift = 0;


void showStrip() {
  FastLED.show();
}

void setPixel(int index, byte red, byte green, byte blue) {
  if(index < 0 || index >= NUM_LEDS)
    return;
  leds[index].r = red;
  leds[index].g = green;
  leds[index].b = blue;
}

void setPixel(int index, CRGB color) {
  if(index < 0 || index >= NUM_LEDS)
    return;
  leds[index] = color;
}

void setPixel(int index, CHSV color) {
  if(index < 0 || index >= NUM_LEDS)
    return;
  leds[index] = color;
}

void setPixel2(int index, byte red, byte green, byte blue) {
  if(index < 0 || index >= NUM_LEDS2)
    return;
  leds2[index].r = red;
  leds2[index].g = green;
  leds2[index].b = blue;
}

void setPixel2(int index, CRGB color) {
  if(index < 0 || index >= NUM_LEDS2)
    return;
  leds2[index] = color;
}

void setPixel2(int index, CHSV color) {
  if(index < 0 || index >= NUM_LEDS2)
    return;
  leds2[index] = color;
}

void setPixelWrap(int index, CHSV color) {
  int i = index;
  if(index >= NUM_LEDS) 
    i = index - NUM_LEDS;
  leds[i] = color;
}

void setPixelWrap(int index, byte red, byte green, byte blue) {
  int i = index;
  if(index >= NUM_LEDS) 
    i = index - NUM_LEDS;
  leds[i] = CRGB(red, green, blue);
}

void fadeLed(int index, int amount) {
  int i = index;
  if(index >= NUM_LEDS) 
    i = index - NUM_LEDS;
  leds[i].fadeToBlackBy(amount);
}

void fadeToBlack(int index, byte fadeValue) {
  if(index < 0 || index >= NUM_LEDS)
    return;
  leds[index].fadeToBlackBy(fadeValue);
}

void fadeAll() { 
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i].nscale8(250); 
  } 
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void setAll(CHSV color) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, color); 
  }
  showStrip();
}

void rotateLedsRight(int cnt, int wait /* =30 */) {

  for(int i=0; i<cnt; i++) {
   CRGB last_led = leds[NUM_LEDS-1];
   memmove(&leds[1], &leds[0], (NUM_LEDS-1)*sizeof(CRGB));
   leds[0] = last_led;
   showStrip();
   delay(wait);
  }
  if(shift + cnt > NUM_LEDS) {
    shift = NUM_LEDS - cnt;
    //Serial.print("Shift: "); Serial.println(shift);
  }
  else 
    shift += cnt;
}

void rotateLedsLeft(int cnt, int wait /*=30 */) {
  
  for(int i=0; i<cnt; i++) {
   CRGB first_led = leds[0];
   memmove(&leds[0], &leds[1], (NUM_LEDS-1)*sizeof(CRGB));
   leds[NUM_LEDS-1] = first_led;
   showStrip();
   delay(wait);
  }

  if(shift-cnt < 0) {
    shift = NUM_LEDS - cnt-1;
    //Serial.print("Shift: "); Serial.println(shift);
  }
  else 
    shift -= cnt;
}

// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } 
  else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
  }
}

// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
  return cur;
}

// Fade an entire array of CRGBs toward a given background color by a given amount
// This function modifies the pixel array in place.
void fadeTowardColor(uint16_t count, const CRGB& bgColor, uint8_t fadeAmount)
{
  for( uint16_t i = 0; i < count; i++) {
    fadeTowardColor(leds[i], bgColor, fadeAmount);
  }
}

byte* Wheel(byte wheelPos) {
  static byte c[3];
  
  if(wheelPos < 85) {
   c[0] = wheelPos * 3;
   c[1] = 255 - wheelPos * 3;
   c[2] = 0;
  } 
  else if(wheelPos < 170) {
   wheelPos -= 85;
   c[0] = 255 - wheelPos * 3;
   c[1] = 0;
   c[2] = wheelPos * 3;
  } 
  else {
   wheelPos -= 170;
   c[0] = 0;
   c[1] = wheelPos * 3;
   c[2] = 255 - wheelPos * 3;
  }
  return c;
}

void one_color_allHSV(int hue, int brightness) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(hue, 255, brightness);
  }
}

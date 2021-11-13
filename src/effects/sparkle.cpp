#include "effects/sparkle.h"

void Sparkle(byte red, byte green, byte blue, int speedDelay) {
  int pixel = random(NUM_LEDS);
  setPixel(pixel,red,green,blue);
  showStrip();
  delay(speedDelay);
  setPixel(pixel,0,0,0);
}

void SnowSparkle(byte red, byte green, byte blue, int sparkleDelay, int speedDelay) {
  setAll(red,green,blue);
  int pixel = random(NUM_LEDS);
  setPixel(pixel,0xff,0xff,0xff);
  showStrip();
  delay(sparkleDelay);
  setPixel(pixel,red,green,blue);
  showStrip();
  delay(speedDelay);
}

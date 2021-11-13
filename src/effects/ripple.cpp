#include "effects/ripple.h"

int fireColor;
int center = NUM_LEDS/2;
static int fireStep = -1;
int maxSteps = NUM_LEDS/2;
float fadeRate = .9;
uint8_t fadeval = 128;
uint8_t myfade = 255; 
uint8_t colour;
 
byte currentBg = random8();
byte nextBg = currentBg;

void Ripple(int speedDelay) {
 
    if (currentBg == nextBg) {
      nextBg = random8();
    }
    else if (nextBg > currentBg) {
      currentBg++;
    } else {
      currentBg--;
    }
    for(int l = 0; l < NUM_LEDS; l++) {
      setPixel(l, CHSV(currentBg, 255, 100));
    }
 
  if (fireStep == -1) {
    center = random(NUM_LEDS);
    fireColor = random8();
    fireStep = 0;
  }
 
  if (fireStep == 0) {
    setPixel(center, CHSV(fireColor, 255, 255));
    fireStep ++;
  }
  else {
    if (fireStep < maxSteps) {
      //Serial.println(pow(fadeRate,fireStep));
 
      setPixel(Wrap(center + fireStep), CHSV(fireColor, 255, pow(fadeRate, fireStep)*255));
      setPixel(Wrap(center - fireStep), CHSV(fireColor, 255, pow(fadeRate, fireStep)*255));
      if (fireStep > 3) {
        setPixel(Wrap(center + fireStep - 3), CHSV(fireColor, 255, pow(fadeRate, fireStep - 2)*255));
        setPixel(Wrap(center - fireStep + 3), CHSV(fireColor, 255, pow(fadeRate, fireStep - 2)*255));
      }
      fireStep++;
    }
    else {
      fireStep = -1;
    }
  }
 
  showStrip();
  delay(speedDelay);
}

void RipplePal(int speedDelay) {

  fadeToBlackBy(leds, NUM_LEDS, fadeval);                             // 8 bit, 1 = slow, 255 = fast

  if(fireStep == maxSteps) {                                           // At the end of the ripples.
      fireStep = -1;
  }
  else {
    switch (fireStep) {
  
      case -1:                                                          // Initialize ripple variables.
        center = random(NUM_LEDS);
        colour = random8();
        fireStep = 0;
        break;
  
      case 0:
        leds[center] = ColorFromPalette(currentPalette, colour, myfade, currentBlending);
        
        fireStep++;
        break;
  
      default:                                                          // Middle of the ripples.
        leds[(center + fireStep + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, myfade/fireStep*2, currentBlending);       // Simple wrap from Marc Miller
        leds[(center - fireStep + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(currentPalette, colour, myfade/fireStep*2, currentBlending);
        fireStep++;                                                         // Next step.
        break;  
    }
  }
  showStrip();
  delay(speedDelay);
}
 
int Wrap(int step) {
  if(step < 0) 
    return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) 
    return step - NUM_LEDS;
  return step;
}

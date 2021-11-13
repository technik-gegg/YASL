#include "effects/inoiseMover.h"

static int16_t dist;                                        // A moving location for our noise generator.
uint8_t       maxChanges = 24;
CRGBPalette16 currentPalette = LavaColors_p;
CRGBPalette16 targetPalette = RainbowColors_p;
TBlendType    currentBlending;                              // NOBLEND or LINEARBLEND 

void Noise8mover(int xscale, int yscale) {
  
  for (int i=0; i<20; i++) {
    uint8_t locn = inoise8(xscale, dist+yscale+i*200);      // Get a new pixel location from moving noise. locn rarely goes below 48 or above 192, so let's remove those ends.
    locn = constrain(locn,48,192);                          // Ensure that the occasional value outside those limits is not used.
    uint8_t index = map(locn,48,192,0,NUM_LEDS-1);         // Map doesn't constrain, so we now map locn to the the length of the strand.
    leds[index] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // Use that value for both the location as well as the palette index colour for the pixel.
  }
  dist += beatsin8(10,1,4);                                 // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
  showStrip();
}

void FillNoise8(int xscale, int yscale) {
  
  for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*xscale, dist+i*yscale) % 255;                // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10,1,4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
  showStrip();
                                                                          // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
}

void Noise16_3() {                                            // no x/y shifting but scrolling along 

  uint16_t scale = 1000;                                      // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = millis()*2;                             // increment z linear

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 7;    // get the noise data and scale it down

    uint8_t index = sin8(noise*3);                            // map led color based on noise data
    uint8_t bri   = noise;

    setPixel(i, ColorFromPalette(currentPalette, index, bri, LINEARBLEND));   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  showStrip();
  
} 

#include "effects/fire.h"

static byte heat[NUM_LEDS];
uint32_t xscale = 20;                                                 // How far apart they are
uint32_t yscale = 3;                                                  // How fast they move
uint8_t  fireIndex = 0;
CRGBPalette16 firePalette = CRGBPalette16(
                                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Maroon,
                                   CRGB::DarkRed, CRGB::Red, CRGB::Red, CRGB::Red,                                   
                                   CRGB::DarkOrange,CRGB::Orange, CRGB::Orange, CRGB::Orange,
                                   CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);


CRGBPalette16 firePaletteBlue = CRGBPalette16(
                                   CRGB::Black, CRGB::Black, CRGB::Black, CRGB::DarkBlue, CRGB::DarkBlue, CRGB::Blue,
                                   CRGB::Blue, CRGB::DarkSlateBlue, CRGB::DarkSlateBlue,                                   
                                   CRGB::CornflowerBlue, CRGB::CornflowerBlue, CRGB::CornflowerBlue,
                                   CRGB::LightBlue, CRGB::LightBlue, CRGB::AliceBlue, CRGB::AliceBlue);
                                   
void Fire(int cooling, int sparking, int speedDelay, bool isLeft) {
  int cnt = NUM_LEDS / 2;
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < cnt; i++) {
    cooldown = random(0, ((cooling * 10) / cnt) + 2);
    
    if(cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for(int k = cnt - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  if(isLeft) {
    for( int j = 0; j < cnt; j++) {
      setPixelHeatColor(j, heat[j] );
    }
  }
  else {
    int n=0;
    for( int j = NUM_LEDS-1; j > cnt; j--) {
      setPixelHeatColor(j, heat[n++] );
    }
  }

  showStrip();
  delay(speedDelay);
}

void setPixelHeatColor (int index, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 1; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(index, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(index, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(index, heatramp, 0, 0);
  }
}

void Noise8_fire() {      // sample from inoise8_fire by A.Tuline
  for(int i = 0; i < NUM_LEDS; i++) {
    fireIndex = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                                    // X location is constant, but we move along the Y at the rate of millis()
    setPixel(i, ColorFromPalette(firePalette, min(i*(fireIndex)>>6, 255), i*255/NUM_LEDS, LINEARBLEND));   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  showStrip();
}  

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

CRGBPalette16 gPal = HeatColors_p;

void Fire2012WithPalette(int cooling, int sparking, bool reverseDirection)
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < sparking ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( reverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

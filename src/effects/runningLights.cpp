#include "effects/runningLights.h"

static int    runlightStep = 0;
static int    runlightPosition = 0;
static bool   runlightCycleDone;
static int    runlightWaveDelay;
CRGB          oldColor = CRGB(0,0,0);
CRGB          newColor;

void RunningLights(byte red, byte green, byte blue, int waveDelay) {

  if(runlightStep == 0) {
    runlightCycleDone = false;
    runlightWaveDelay = waveDelay;
    newColor = ColorFromPalette(currentPalette, random(0,4)); //CRGB(red, green, blue);
  }
  oldColor = fadeTowardColor(oldColor, newColor, 64);
  
  if(runlightStep < NUM_LEDS*2) {
      runlightPosition++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        float level = sin(i+runlightPosition) * 127 + 128;
        
        setPixel(i,((sin(i+runlightPosition) * 127 + 128)/255) * oldColor.red,
                   ((sin(i+runlightPosition) * 127 + 128)/255) * oldColor.green,
                   ((sin(i+runlightPosition) * 127 + 128)/255) * oldColor.blue);
        
        // sine wave, 3 offset waves make a rainbow!
        //setPixel(i,level,0,0);
      }
      runlightStep++;
      showStrip();
      delay(runlightWaveDelay);
  }
  else {
    runlightStep = 0;
    runlightCycleDone = true;
  }
  //Serial.print(runlightPosition); Serial.print(" "); Serial.println(runlightStep);
}

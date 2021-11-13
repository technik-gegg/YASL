#pragma once

#include "Config.h"
#include "arduinoFFT.h" 

#define SAMPLES                     256         // Must be a power of 2
#define SAMPLING_FREQUENCY          40000       // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE                   1          // Depending on your audio source level, you may need to increase this value
#define TH_NOISE                    10  
#define DC_OFFSET                   5
#define TH_ROTATE                   210
#define TH_ROTATE_MAX               TH_ROTATE+10
#define TH_ROTATE_MIN               TH_ROTATE-10

extern unsigned int  sampling_period_us;
extern CRGB new_color[];
extern CRGB old_color[];
extern CRGBPalette16 currentPalette;

void initSound();
void getAudio();
void animateSound();
void setBandFreq(int band, double dsize);

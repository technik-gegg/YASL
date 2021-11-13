#include "effects/audioAnimator.h"


double        vReal[SAMPLES];
double        vImag[SAMPLES];
int           fpeak[NUM_BARS];
bool          isIdle = false;
bool          useRotation = true;
unsigned int  sampling_period_us;
unsigned long msecs;
unsigned long firstQuiet;
unsigned int  lastRotation=0;
CHSV          bar_color[NUM_BARS];
CRGB          old_color[NUM_BARS];
CRGB          new_color[NUM_BARS];
char          b[128];
arduinoFFT    fft = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

const TProgmemRGBPalette16 EqualizerColors_p FL_PROGMEM =
{
    0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
    0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
    0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
    0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B
};

void initSound() {
  bar_color[0] = CHSV(HUE_RED, 255, 255);
  bar_color[1] = CHSV(HUE_ORANGE, 255, 255);
  bar_color[2] = CHSV(HUE_YELLOW, 255, 255);
  if(NUM_BARS > 3 && NUM_BARS <7) {
    bar_color[3] = CHSV(HUE_GREEN, 255, 255);
    bar_color[4] = CHSV(HUE_AQUA, 255, 255);
  }
  if(NUM_BARS > 5) {
    bar_color[5] = CHSV(HUE_BLUE, 255, 255);
    bar_color[6] = CHSV(HUE_PURPLE, 255, 255);
  }
}
void animateSound() 
{
  for(int bar=0; bar<NUM_BARS; bar++) {

    int dsize = map(fpeak[bar], 0, 255, 0, LEDS_IN_GROUP);
    if(dsize > LEDS_IN_GROUP)
      dsize = LEDS_IN_GROUP;

    if(useRotation) {
      if(bar==0 && fpeak[bar] >= TH_ROTATE_MIN && millis()-lastRotation > 4000) {
        int len = fpeak[bar] >= TH_ROTATE_MAX ? NUM_LEDS/2 : fpeak[bar] >= TH_ROTATE_MIN ? NUM_LEDS/3 : NUM_LEDS/4;
        if(random(0,100) % 2)
          rotateLedsRight(len);
        else
          rotateLedsLeft(len);
        lastRotation = millis();
        return;
      }
    }
    if(dsize > LEDS_IN_GROUP)
      dsize = LEDS_IN_GROUP;
    int st = bar * LEDS_IN_GROUP + shift;
    //Serial.print(dsize); Serial.print(",");
    
    for(int i=0; i< dsize; i++) {
      setPixelWrap(st+i, bar_color[bar]);
    }
    
    for(int i=dsize; i< LEDS_IN_GROUP; i++) {
      fadeLed(st+i, i*30+100);
    }
  }
  showStrip();
  //Serial.println();
}


void getAudio() 
{
  double v, f;

  for(int i=0; i<NUM_BARS; i++) {
    fpeak[i]=0;
  }
  for (int i = 0; i < SAMPLES; i++) {
    msecs = micros();         // overflows after around 70 minutes!
    if(msecs > 4293000000) {
      delay(3000);
      msecs = micros();       // compensate overflow
    }
    vReal[i] = analogRead(MIC_PIN) - DC_OFFSET;
    vImag[i] = 0;
    // Serial.print("50,"); Serial.print(vReal[i]); Serial.print(","); Serial.println("256");
    while (micros() < (msecs + sampling_period_us)) {  }
  }
  fft.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  fft.Compute(FFT_FORWARD);
  fft.ComplexToMagnitude();
  //fft.MajorPeak(&f, &v);
  //Serial.print(f/5, 6);  Serial.print(", ");  Serial.print(v, 6);
  
  for (int i = 2; i < (SAMPLES/2); i++){
    //Serial.print((int)vReal[i]); Serial.print(",");
    if (vReal[i] > TH_NOISE) {
      if(NUM_BARS >= 7) {
        if (i >=6   && i<=10) setBandFreq(0, vReal[i]/AMPLITUDE); // 468-780 Hz
        if (i >=10  && i<=13) setBandFreq(1, vReal[i]/AMPLITUDE); // 780-1014 Hz
        if (i >=13  && i<=15) setBandFreq(2, vReal[i]/AMPLITUDE); // 1014-1170 Hz
        if (i >=15  && i<=18) setBandFreq(3, vReal[i]/AMPLITUDE); // 1170-1404 Hz
        if (i >=18  && i<=22) setBandFreq(4, vReal[i]/AMPLITUDE); // 1404-1716 Hz
        if (i >=22  && i<=30) setBandFreq(5, vReal[i]/AMPLITUDE); // 1716-2340 Hz
        if (i >=30  && i<=45) setBandFreq(6, vReal[i]/AMPLITUDE); // 2340-3510 Hz
      }
      if(NUM_BARS == 5) {
        if (i >=10  && i<=13) setBandFreq(0, vReal[i]/AMPLITUDE); // 780-1014 Hz
        if (i >=13  && i<=15) setBandFreq(1, vReal[i]/AMPLITUDE); // 1014-1170 Hz
        if (i >=15  && i<=18) setBandFreq(2, vReal[i]/AMPLITUDE); // 1170-1404 Hz
        if (i >=18  && i<=22) setBandFreq(3, vReal[i]/AMPLITUDE); // 1404-1716 Hz
        if (i >=22  && i<=30) setBandFreq(4, vReal[i]/AMPLITUDE); // 1716-2340 Hz
      }
      if(NUM_BARS == 3) {
        if (i >=13  && i<=15) setBandFreq(0, vReal[i]/AMPLITUDE); // 1014-1170 Hz
        if (i >=15  && i<=18) setBandFreq(1, vReal[i]/AMPLITUDE); // 1170-1404 Hz
        if (i >=18  && i<=22) setBandFreq(2, vReal[i]/AMPLITUDE); // 1404-1716 Hz
      }
    }
  }
  
  double freq = 0; 
  for (byte band = 0; band < NUM_BARS; band++) {
    freq += fpeak[band];
    // sprintf(b, "%3d, ", fpeak[band]);
    // Serial.print(b);
  }
  //Serial.println();

  if(freq == 0 && firstQuiet == 0) {
      firstQuiet = millis();
  }
  else if(freq == 0) {
    if(millis()-firstQuiet > 20000) {
      isIdle = true;
    }
  }
  else if(freq > 0) {
    firstQuiet = 0;
    isIdle = false;
  }
}

void setBandFreq(int band, double dsize){
  
  if(band < 0 || band >= NUM_BARS)
    return;
  if(fpeak[band] < (int)dsize)
    fpeak[band] = (int)dsize;
}

 

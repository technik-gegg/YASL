#include "fileUtils.h"
#include "effectlist.h"

extern int    wipeSpeed, fadeSpeed, twinkleSpeed, theaterCaseSpeed, 
              meteorRainSpeed, rippleSpeed, blendSpeed, cylonSpeed;

void readEffect() {
#if defined(USE_FS)
  File in = LittleFS.open(EFFECT_FILE, "r");
  if (!in) {
    Serial.println("Effect file open failed!");
    selectedEffect = DEFAULT_EFFECT;
  }
  else {
      selectedEffect = (byte)in.read();
      in.close();
      if(selectedEffect == -1 || selectedEffect >= MAX_EFFECTS)
        selectedEffect = DEFAULT_EFFECT;
  }
#endif
}

void writeEffect() {
#if defined(USE_FS)
  File out = LittleFS.open(EFFECT_FILE, "w");
  if (!out) {
    Serial.println("Effect file open failed!");
  }
  else {
      out.write(newEffect);
      out.close();
  }
#endif
}

void readSpeeds() {
#if defined(USE_FS)
  File in = LittleFS.open(SPEEDS_FILE, "r");
  if (!in) {
    Serial.println("Speeds file open failed!");
  }
  else {
      
      wipeSpeed         = in.read();
      fadeSpeed         = in.read();
      twinkleSpeed      = in.read();
      theaterCaseSpeed  = in.read();
      meteorRainSpeed   = in.read();
      rippleSpeed       = in.read();
      blendSpeed        = in.read();
      cylonSpeed        = in.read();
      in.close();
  }
#endif
}


void writeSpeeds() {
#if defined(USE_FS)
  File out = LittleFS.open(SPEEDS_FILE, "w");
  if (!out) {
    Serial.println("Speeds file open failed!");
  }
  else {
      out.write(wipeSpeed);
      out.write(fadeSpeed);
      out.write(twinkleSpeed);
      out.write(theaterCaseSpeed);
      out.write(meteorRainSpeed);
      out.write(rippleSpeed);
      out.write(blendSpeed);
      out.write(cylonSpeed);
      out.close();
  }
#endif
}

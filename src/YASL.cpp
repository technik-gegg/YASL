#include "Config.h"
#include "fileUtils.h"
#include "effectlist.h"
#include "webserver.h"

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>


CRGBPalette16   paletts[] = {
                  CloudColors_p,
                  RainbowColors_p,
                  OceanColors_p,
                  ForestColors_p,
                  LavaColors_p,
                  PartyColors_p,
                  HeatColors_p
                };
String          cmd;
byte            selectedEffect;
byte            newEffect;
byte            lastEffect;
bool            isMuted = false;
bool            isPowered = true;
bool            smoothBlink = false;
bool            fireReverseDirection = false;
bool            isLearning = false;
bool            inhibitBroadcast = false;
int             twinkleColor = 164;
int             paletteIndex = 0;                          
int             fireCooling = 70;
int             fireSparking = 120;
int             numBalls = 3;
int             btnPress = 0;
int             btnCmd = 0;
int             wipeSpeed = 70;
int             fadeSpeed = 10;
int             twinkleSpeed = 30;
int             theaterCaseSpeed = 105;
int             meteorRainSpeed  = 50;
int             cylonSpeed = 90;
int             rippleSpeed = 50;
int             blendSpeed = 40;
uint8_t         brightness = 255;
unsigned        millisCurrent;
unsigned        millisLast;
uint32_t        irCodes[MAX_IRCODES] {
                  0x20,   // Arrow UP Button
                  0x21,   // Arrow DOWN Button
                  0x11,   // Arrow LEFT Button
                  0x10,   // Arrow RIGHT Button
                  0x2C,   // Menu Button
                  0x0C,   // Power Button
                  0x0D,   // Mute Button 
                  0x0F,   // PiP Button (Save)
                  0x2B,   // AV/TV Button (Music)
                  0x0A    // Minus Button
                };
const char*     learnInstructions[] = {
                  "Arrow UP Button",
                  "Arrow DOWN Button",
                  "Arrow LEFT Button",
                  "Arrow RIGHT Button",
                  "Menu Button",
                  "Power Button",
                  "Mute Button",
                  "PIP (Save) Button",
                  "AV/TV (Music) Button",
                  "Minus Button"
                };              

IRrecv          irReceiver(IR_PIN);
decode_results  results;


void incrementEffect() {
  if(newEffect < MAX_EFFECTS-1)
    newEffect++;
  else 
    newEffect = 0;
}

void decrementEffect() {
  if(newEffect > 0) 
    newEffect--;
  else
    newEffect = MAX_EFFECTS-1;
}

void setEffect(int number) {
  if(number == 88) {
    wifiMgr.resetSettings();                // wipe credentials
    ESP.restart();
  }
  if(number >=0 && number < MAX_EFFECTS) 
    newEffect = (byte)number;
}

void decrementPalette() {
  targetPalette = paletts[paletteIndex];
  paletteIndex--;
  if(paletteIndex < 0)
    paletteIndex = 6;
  currentPalette = paletts[paletteIndex];
}

void incrementPalette() {
  targetPalette = paletts[paletteIndex];
  paletteIndex++;
  if(paletteIndex > 6)
    paletteIndex = 0;
  currentPalette = paletts[paletteIndex];
}

void decrementParam(int* target, int steps) {
  *target -= steps;
  if(*target < steps)
    *target = steps;
  Serial.printf("Speed/Param now: %d\n", *target);
}

void incrementParam(int* target, int steps, int max) {
  *target += steps;
  if(*target > max)
    *target = max;
  Serial.printf("Speed/Param now: %d\n", *target);
}

void IRAM_ATTR changeEffect(void) {
  millisCurrent = millis();
  if(millisCurrent - millisLast > 100) {
    millisLast = millis();
    incrementEffect();
  }
}

void plainColor(CRGB color) {
  fill_solid(leds, NUM_LEDS, color);
  if(LED2_PIN != -1)
    fill_solid(leds2, NUM_LEDS2, color);
  showStrip();
}

void plainColor(CHSV color) {
  fill_solid(leds, NUM_LEDS, color);
  if(LED2_PIN != -1)
    fill_solid(leds2, NUM_LEDS2, color);
  showStrip();
}

void changeColor() {
  for(int i=0; i < NUM_BARS; i++) {
    old_color[i] = new_color[i];
    new_color[i] = CRGB(random8(), random8(), random8());
  }
}

int findButton(uint32_t btn) {
  int index =-1;
  for(int i=0; i< MAX_IRCODES; i++) {
    if(irCodes[i] == btn) {
      index = i;
      break; 
    }
  }
  return index;
}

void handleIRbutton(int btn) {
  switch(btn) {
    case IR_PIP:
      // store currently set effect for next power up
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      showStrip();
      delay(250);
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      showStrip();
      delay(250);
      writeEffect();
      writeSpeeds();
      selectedEffect = -1;
      Serial.printf("Settings stored in file system.\n");
      break;
    
    case IR_UP:
      incrementEffect();
      break;
    
    case IR_DOWN:
      decrementEffect();
      break;
    
    case IR_AVTV:
      // toggle between Music and last effect
      if(newEffect == E_MUSIC) {
        newEffect = lastEffect;
      }
      else {
        lastEffect = selectedEffect;
        newEffect = E_MUSIC;
      }
      break;

    case IR_LEFT:
      // change options on some effects
      switch(selectedEffect) {
        case E_COLORWIPE:
          incrementParam(&wipeSpeed, 10, 500);
          break;

        case E_FADE:
          incrementParam(&fadeSpeed, 10, 500);
          break;

        case E_TWINKLE:
        case E_TWINKLE2:
          incrementParam(&twinkleSpeed, 10, 500);
          break;

        case E_RIPPLE:
        case E_RIPPLE2:
          incrementParam(&rippleSpeed, 10, 500);
          break;

        case E_BLENDWAVE:
          incrementParam(&blendSpeed, 10, 500);
          break;

        case E_THEATERCASE:
        case E_THEATERCASE2:
          incrementParam(&theaterCaseSpeed, 10, 500);
          break;

        case E_METEORRAIN:
        case E_METEORRAIN2:
          incrementParam(&meteorRainSpeed, 10, 500);
          break;
        
        case E_CYLONBOUNCE:
          incrementParam(&cylonSpeed, 5, 250);
          break;

        case E_FIRE: 
          incrementParam(&fireCooling, 5, 250);
          break;

        case E_BOUNCINGBALLS: 
          incrementParam(&numBalls, 1, MAX_BALLS);
          break;

        case E_WHITE:
        case E_BLUE:
        case E_GREEN:
        case E_YELLOW:
        case E_RED:
        case E_ORANGE:
        case E_PINK:
        case E_AQUA:
        case E_PURPLE:
          smoothBlink = !smoothBlink;
          break;

        default:       
          decrementPalette();
          break;

      }
      break;

    case IR_RIGHT:
      // change options on some effects
      switch(selectedEffect) {
        case E_COLORWIPE:
          decrementParam(&wipeSpeed, 10);
          break;

        case E_FADE:
          decrementParam(&fadeSpeed, 10);
          break;

        case E_TWINKLE:
        case E_TWINKLE2:
          decrementParam(&twinkleSpeed, 10);
          break;

        case E_RIPPLE:
        case E_RIPPLE2:
          decrementParam(&rippleSpeed, 10);
          break;

        case E_BLENDWAVE:
          decrementParam(&blendSpeed, 10);
          break;

        case E_THEATERCASE:
        case E_THEATERCASE2:
          decrementParam(&theaterCaseSpeed, 10);
          break;
          
        case E_METEORRAIN:
        case E_METEORRAIN2:
          decrementParam(&meteorRainSpeed, 10);
          break;

        case E_CYLONBOUNCE:
          decrementParam(&cylonSpeed, 5);
          break;

        case E_FIRE: 
          decrementParam(&fireCooling, 5);
          break;

        case E_BOUNCINGBALLS: 
          decrementParam(&numBalls, 1);
          break;

        case E_WHITE:
        case E_BLUE:
        case E_GREEN:
        case E_YELLOW:
        case E_RED:
        case E_ORANGE:
        case E_PINK:
        case E_AQUA:
        case E_PURPLE:
            smoothBlink = !smoothBlink;
          break;

        default: 
          incrementPalette();
          break;
      }
      break;

    case IR_MUTE:
      isMuted = !isMuted;
      break;

    case IR_POWER:
      isPowered = !isPowered;
      break;
    
    case IR_MINUS:
      if(selectedEffect == E_FIRE || selectedEffect == E_FIRE2) {
        // turn fire upside down
        fireReverseDirection = !fireReverseDirection;
      }
      else if(selectedEffect == E_TWINKLE2)
        {
          if(twinkleColor < 224)
            twinkleColor += 32;
          else 
            twinkleColor = 0;
        }
      break;
    
    default:
      Serial.printf("Unknown button: %d\n", btn);
      break;
  }
}

bool handlePower() {
  if(!isPowered) {
    // emulate powering off
    fadeTowardColor(NUM_LEDS, CRGB::Black, 1);
    if(LED2_PIN != -1) {
      uint8_t brightness = beatsin8(10, 0, 255);
      fill_solid(leds2, NUM_LEDS2, CHSV(HUE_RED, 255, brightness));
    }
    showStrip();
    return true;
  }
  else {
    if(LED2_PIN != -1) {
      fill_solid(leds2, NUM_LEDS2, CRGB::Black);
      showStrip();
    }
  }
  return false;
}

void handleMute() {
  if(isMuted) {
    fadeTowardColor(NUM_LEDS, CRGB::Black, 1);
    if(LED2_PIN != -1 && isPowered) {
      uint8_t brightness = beatsin8(25, 0, 255);
      fill_solid(leds2, NUM_LEDS2, CHSV(HUE_AQUA, 255, brightness));
    }
    showStrip();
  }
}

void setup(){
  
  Serial.begin(115200);
  Serial.printf("\nStarting...\n");
  #if defined(USE_FS)
  if(LittleFS.begin()) {
    Serial.println("FileSystem init... ok");
  }
  else {
    Serial.println("FileSystem init... failed");
  }  
  #endif
  Serial.println("FastLED init...");
  if(LED2_PIN != -1)
    FastLED.addLeds<WS2812B, LED2_PIN, GRB>(leds2, NUM_LEDS2);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setDither(false);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  if(LED2_PIN != -1)
    fill_solid(leds2, NUM_LEDS2, CRGB::Black);

  if(BTN_PIN != -1) {
    Serial.println("Button init...");
    digitalWrite (BTN_PIN, INPUT_PULLUP);  
    attachInterrupt (digitalPinToInterrupt (BTN_PIN), changeEffect, FALLING); 
  }
  Serial.println("IR-Receiver init...");
  digitalWrite (IR_PIN, INPUT_PULLUP);  
  irReceiver.enableIRIn();
  
  sampling_period_us = round(130000*(1.0/SAMPLING_FREQUENCY));
  Serial.printf("Mic sampling period: %d uS\n", sampling_period_us);
  
  readEffect();
  newEffect = selectedEffect;
  readSpeeds();
  readIrCodes();
  initSound();
  changeColor();
  Serial.println("Webserver init...");
  initWebserver();
}

void learnRemote() {
  int i = 0;
  bool status;
  irparams_t irParams;
  decode_results res;
  uint32_t lastCode = 0;
  uint32_t newCodes[MAX_IRCODES];
  unsigned long start = millis();

  setAll(0, 0, 0);
  setPixel(0, CHSV(HUE_AQUA, 255, 255));
  setPixel(1, CHSV(HUE_GREEN,  255, 255));
  setPixel(2, CHSV(HUE_AQUA, 255, 255));
  showStrip();
  Serial.printf("\n\n*** LEARNING IR-CODES ***\n");
  isLearning = true;
  for(int n=0; n< MAX_IRCODES; n++) {
    newCodes[n] = 0;
    rotateLedsRight(1);
    delay(50);
  }
  rotateLedsRight(1);
  start = millis();
  do {
    Serial.printf("Please press now the %s\t\t\t", learnInstructions[i]);
    do {
      status = irReceiver.decode(&res);
      delay(150);
      if(millis() - start > 10000) {
        isLearning = false;
        Serial.printf("\n\n*** TIMED OUT, ABORTING ***\n");
        return;
      }
    } while(!status || res.rawlen == 0 || res.command == lastCode || res.repeat);
    irReceiver.resume();
    start = millis();
    if(res.decode_type != RC5 && res.decode_type != RC5X) {
      Serial.printf("Not a RC5/RC5X IR-Signal. Try another remote.\n");
      continue;
    }
    for(int n=0; n< i; n++) {
      if(newCodes[n] == res.command) {
        Serial.printf("IR-Code: 0x%08X\n\n", res.command);
        Serial.printf("Button already in use, try again.\n");
        continue;
      }
    }
    Serial.printf("using IR-Code: 0x%08X\n", res.command);
    newCodes[i] = res.command;
    lastCode = newCodes[i];
    i++;
    rotateLedsRight(NUM_LEDS-2,  15);
  } while(i < MAX_IRCODES);
  for(int n=0; n< MAX_IRCODES; n++) {
    irCodes[n] = newCodes[n];
  }
  writeIrCodes();
  Serial.printf("*** DONE ***\n");
  isLearning = false;
}

void serialEvent() {
  while (Serial.available()) {
    char in = (char)Serial.read();
    if(in != '\n') {
      cmd += in;
    }
    else {
        int effect = atoi(cmd.c_str());
        if(effect == 99) {
          learnRemote();
        }
        setEffect(effect);
    }
  }
}

void loop() { 

  loopWebserver();
  if(isLearning)
    return;

  if (irReceiver.decode(&results)) {
    if(results.decode_type == RC5 || results.decode_type == RC5X) {
      Serial.printf("IR-Code:    0x%08X\n", results.command);
      uint32_t btn = results.command;
      if(btn >= 0 && btn <= 9) {
        if(btnPress == 0) {
          btnCmd = btn*10;
          btnPress++;
        }
        else  {
          btnPress = 0;
          btnCmd += btn;
          setEffect(btnCmd);
        }
      }
      else {
        btnCmd = 0;
        btnPress = 0;
        handleIRbutton(findButton(btn));
      }
    }
    else {
      Serial.printf("Not a RC5/RC5X IR-Signal detected.\n");
    }
    delay(100);
    irReceiver.resume();  // Receive the next value
  }

  if(newEffect != selectedEffect) {
    selectedEffect = newEffect;
    Serial.printf("Effect now: %s\n", allEffects[selectedEffect].displayName);
    fadeTowardColor(NUM_LEDS, CRGB::Black, 5);
    showStrip();

    if(selectedEffect == E_MUSIC) {
      // blink to signal Equalizer is active
      setPixel(NUM_LEDS/2,   CHSV(160, 255, 255));
      setPixel(NUM_LEDS/2-1, CHSV(160, 128, 255));
      setPixel(NUM_LEDS/2+1, CHSV(160, 128, 255));
      rotateLedsRight(NUM_LEDS);
    }
    broadcastEffect(newEffect);
  }

  if(handlePower())
    return;

  handleMute();

  EVERY_N_SECONDS(5) {
    changeColor();
  }

  EVERY_N_MILLISECONDS(2019) {
    shift = (shift > NUM_LEDS) ? 0 : shift++;
  }
  
  EVERY_N_MILLISECONDS(20) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }
  
  brightness = beatsin8(10, 0, 255);
  if(!smoothBlink)
    brightness = 255;

  if(selectedEffect >=0 && selectedEffect < MAX_EFFECTS)
    allEffects[selectedEffect].func();

}

#pragma once

#define FASTLED_ESP8266_RAW_PIN_ORDER       // IMPORTANT!
#define FASTLED_INTERNAL

#include "Config.h"
#include "FastLED.h"

extern CRGB       leds[];
extern CRGB       leds2[];
extern byte       shift;

void showStrip();
void setPixel(int index, byte red, byte green, byte blue);
void setPixel(int index, CHSV color);
void setPixel(int index, CRGB color);
void setPixel2(int index, byte red, byte green, byte blue);
void setPixel2(int index, CRGB color);
void setPixel2(int index, CHSV color);
void setPixelWrap(int index, CHSV color);
void setPixelWrap(int index, byte red, byte green, byte blue);
void fadeLed(int index, int amount);
void fadeToBlack(int index, byte fadeValue);
void fadeAll();
void setAll(byte red, byte green, byte blue);
void setAll(CHSV color);
void rotateLedsRight(int cnt, int wait=30);
void rotateLedsLeft(int cnt, int wait=30);
CRGB fadeTowardColor(CRGB& cur, const CRGB& target, uint8_t amount);
void fadeTowardColor(uint16_t N, const CRGB& bgColor, uint8_t fadeAmount);
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount);
byte* Wheel(byte wheelPos);

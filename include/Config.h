#pragma once

#if __cplusplus >= 201703L
#define register        // keyword 'register' is banned with c++17; 
                        // Add this few lines to FastLED.h to get rid of the compiler warnings
                        // "ISO C++17 does not allow 'register' storage..."
#endif


#include "ledUtils.h"

#define VERSION                     "1.0.2"
#define CIRCLE_STRIP                0
#define NUM_LEDS                    15
#define NUM_LEDS2                   3
#define NUM_BARS                    5
#define LEDS_IN_GROUP               (int)NUM_LEDS/NUM_BARS
#define BRIGHTNESS                  200
#define MILLI_AMPS                  1500

#define LED_PIN                     D4     // (GPIO2) 
#define LED2_PIN                    D5     // (GPIO14) 
#define IR_PIN                      D1     // (GPIO5) 
#define MIC_PIN                     A0
#define BTN_PIN                     -1
#define USE_FS                      1


// Indices for RC5 Codes
enum _IRCodes {
    IR_UP,
    IR_DOWN,
    IR_LEFT,
    IR_RIGHT,
    IR_MENU,
    IR_POWER,
    IR_MUTE,
    IR_PIP,
    IR_AVTV,
    IR_MINUS
};

#define MAX_IRCODES                 10


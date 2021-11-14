#pragma once

#if __cplusplus >= 201703L
#define register        // keyword 'register' is banned with c++17; 
                        // Add this few lines to FastLED.h to get rid of the compiler warnings
                        // "ISO C++17 does not allow 'register' storage..."
#endif


#include "ledUtils.h"

#define VERSION                     "1.0.1"
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


// RC5 Codes for IR Remote
#define IR_UP                       0x20    // Arrow UP Button
#define IR_DOWN                     0x21    // Arrow DOWN Button
#define IR_LEFT                     0x11    // Arrow LEFT Button
#define IR_RIGHT                    0x10    // Arrow RIGHT Button
#define IR_MENU                     0x2C    // Menu Button
#define IR_AVTV                     0x2B    // AV Button
#define IR_POWER                    0x0C    // Power Button
#define IR_MUTE                     0x0D    // Mute Button
#define IR_PIP                      0x0F    // Picture-In-Picture Button
#define IR_PICTURE                  0x38    // Picture Button
#define IR_CALENDAR                 0x1C    // Calendar Button
#define IR_PSIZE                    0x1E    // Picture Size Button
#define IR_MINUS                    0x0A    // Minus Button


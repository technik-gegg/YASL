
#include "effectlist.h"

extern bool     isMuted;
extern bool     fireReverseDirection;
extern int      fireCooling, fireSparking, twinkleColor, numBalls;
extern int      wipeSpeed, fadeSpeed, twinkleSpeed, theaterCaseSpeed, 
                meteorRainSpeed, rippleSpeed, blendSpeed, cylonSpeed;
extern uint8_t  brightness;

extern void     plainColor(CHSV color);

void _FadeInOut() {
    FadeInOut(random8(), random8(), random8(), fadeSpeed);
}

void _Music() {
    if(!isMuted) {
        getAudio();
        animateSound();
    }
}

void _RgbLoop() {
    RgbLoop(random(3,50));
}

void _CylonBounce() {
    CylonBounce(random8(), random8(), random8(), 6, cylonSpeed, 150);
}

void _CylonHue() { 
    CylonHue(cylonSpeed); 
}

void _MeteorRain1() {
    MeteorRain(0x20,0xb8,0xf1, 12, 64, true, meteorRainSpeed, false);
}

void _MeteorRain2() {
    MeteorRain(random8(),random8(),random8(), 10, 64, true, meteorRainSpeed, random8());
}

void _TwinkleRandom() {
    TwinkleRandom(random(NUM_LEDS/3, NUM_LEDS/3*2), twinkleSpeed);
}

void _Twinkle(){ 
    Twinkle(CHSV(twinkleColor, 255, 164), random(NUM_LEDS/3,NUM_LEDS), twinkleSpeed, false);
}

void _FillNoise8() { 
    FillNoise8(); 
}

void _RipplePal() { 
    RipplePal(rippleSpeed); 
}

void _BlendWave() {
    BlendWave(blendSpeed); 
}

void _Ripple() { 
    Ripple(rippleSpeed); 
}

void _RunningLights() {
    RunningLights(random8(),random8(),random8(), random(20, 100));
}

void _ColorWipe() {
    if(wipeCycle % 2 == 0)
        ColorWipe(random8(),random8(),random8(), wipeSpeed);
    else
        ColorWipe(0,0,0, wipeSpeed);
}

void _RainbowCycle() {
    RainbowCycle(random(10,30));
}

void _TheaterChase() {
    CRGB color = ColorFromPalette(currentPalette, 1, 255, NOBLEND);
    TheaterChase(color.red, color.green, color.blue, theaterCaseSpeed);
}

void _TheaterChaseRainbow() {
    TheaterChaseRainbow(theaterCaseSpeed);
}

void _Fire() {
#if CIRCLE_STRIP == 1
    randomSeed(analogRead(MIC_PIN));
    Fire(fireCooling, fireSparking, random(30,60), true);
    Fire(fireCooling, fireSparking, random(30,60), false);
#else
    gPal = firePalette;
    Fire2012WithPalette(fireCooling, fireSparking, fireReverseDirection);
    showStrip();
    FastLED.delay(1500 / 60);
#endif
}

void _Fire2() {
    gPal = firePaletteBlue;
    Fire2012WithPalette(fireCooling, fireSparking, fireReverseDirection);
    showStrip();
    FastLED.delay(1500 / 60);
}

void _BouncingColoredBalls() { 
    BouncingColoredBalls(numBalls); 
}

void _SolidWhite()  { plainColor(CHSV(HUE_BLUE,       0, brightness)); }
void _SolidBlue()   { plainColor(CHSV(HUE_BLUE,     255, brightness)); }
void _SolidGreen()  { plainColor(CHSV(HUE_GREEN,    255, brightness)); }
void _SolidYellow() { plainColor(CHSV(HUE_YELLOW,   255, brightness)); }
void _SolidRed()    { plainColor(CHSV(HUE_RED,      255, brightness)); }
void _SolidOrange() { plainColor(CHSV(HUE_ORANGE,   255, brightness)); }
void _SolidPink()   { plainColor(CHSV(HUE_PINK,     255, brightness)); }
void _SolidAqua()   { plainColor(CHSV(HUE_AQUA,     255, brightness)); }
void _SolidPurple() { plainColor(CHSV(HUE_PURPLE,   255, brightness)); }


Effect allEffects[]  = {
    { E_FADE,           "Fade In/Out",        _FadeInOut },
    { E_MUSIC,          "Music",              _Music },
    { E_RGBLOOP,        "RGB Loop",           _RgbLoop },
    { E_CYLONBOUNCE,    "Cylon Bounce",       _CylonBounce },
    { E_METEORRAIN,     "Meteor Rain 1",      _MeteorRain1 },
    { E_TWINKLE,        "Twinkle 1",          _TwinkleRandom },
    { E_RGBNOISE,       "RGB Noise",          _FillNoise8 },
    { E_RIPPLE,         "Ripple 1",           _RipplePal },
    { E_BLENDWAVE,      "Blend Wave",         _BlendWave },
    { E_RUNNINGLIGHTS,  "Running Lights",     _RunningLights },
    { E_COLORWIPE,      "Color Wipe",         _ColorWipe },
    { E_RAINBOW,        "Rainbow",            _RainbowCycle },
    { E_THEATERCASE,    "Theater Chase 1",    _TheaterChase },
    { E_THEATERCASE2,   "Theater Chase 2",    _TheaterChaseRainbow },
    { E_FIRE,           "Fire",               _Fire },
    { E_FIRE2,          "Plasma Fire",        _Fire2 },
    { E_BOUNCINGBALLS,  "Bouncing Balls",     _BouncingColoredBalls },
    { E_METEORRAIN2,    "Meteor Rain 2",      _MeteorRain2 },
    { E_CYLONHUE,       "Cylon Hue",          _CylonHue },
    { E_RIPPLE2,        "Ripple 2",           _Ripple },
    { E_TWINKLE2,       "Twinkle 2",          _Twinkle },
    { E_WHITE,          "Solid White",        _SolidWhite  },
    { E_BLUE,           "Solid Blue",         _SolidBlue },
    { E_GREEN,          "Solid Green",        _SolidGreen },
    { E_YELLOW,         "Solid Yellow",       _SolidYellow },
    { E_RED,            "Solid Red",          _SolidRed },
    { E_ORANGE,         "Solid Orange",       _SolidOrange },
    { E_PINK,           "Solid Pink",         _SolidPink },
    { E_AQUA,           "Solid Aqua",         _SolidAqua },
    { E_PURPLE,         "Solid Purple",       _SolidPurple }
};


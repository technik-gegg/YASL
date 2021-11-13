
#pragma once

#include "Config.h"
#include "effects.h"

// EFFECTS
enum _FX {
    E_FADE,
    E_MUSIC,
    E_RGBLOOP,
    E_CYLONBOUNCE,
    E_METEORRAIN,
    E_TWINKLE,
    E_RGBNOISE,
    E_RIPPLE,
    E_BLENDWAVE,
    E_RUNNINGLIGHTS,
    E_COLORWIPE,
    E_RAINBOW,
    E_THEATERCASE,
    E_THEATERCASE2,
    E_FIRE,
    E_FIRE2,
    E_BOUNCINGBALLS,
    E_METEORRAIN2,
    E_CYLONHUE,
    E_RIPPLE2,
    E_TWINKLE2,
    E_WHITE,
    E_BLUE,
    E_GREEN,
    E_YELLOW,
    E_RED,
    E_ORANGE,
    E_PINK,
    E_AQUA,
    E_PURPLE,
    MAX_EFFECTS
};

#define DEFAULT_EFFECT              E_RAINBOW

typedef struct {
    int index;
    const char* displayName;
    void (*func)();
} Effect;

extern Effect allEffects[];

#pragma once

#include "Config.h"
#include <LittleFS.h>

#define EFFECT_FILE "Effect.dat"
#define SPEEDS_FILE "Speeds.dat"
#define IRCODE_FILE "IRCodes.dat"

extern byte selectedEffect;
extern byte newEffect;

void readEffect();
void writeEffect();
void readSpeeds();
void writeSpeeds();
void readIrCodes();
void writeIrCodes();

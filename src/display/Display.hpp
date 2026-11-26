#pragma once

#include <LedControl.h>
#include "physics/Physics.hpp"

extern uint32_t pause[16];
extern uint32_t loss[16];
extern uint32_t score[16];

bool compareCoordinates(Coordinate*, Coordinate*);
void flushBuffer(LedControl*, uint32_t[16]);
void setPixel(uint32_t[16], const Coordinate*, bool);
bool getPixel(uint32_t[16], const Coordinate*);
bool checkRowWin(uint32_t[16], uint8_t);
void setRow(LedControl* lc, uint32_t[16], uint8_t, bool);
bool rowContains(uint32_t[16], uint8_t, bool);
void moveRow(uint32_t[16], uint8_t, uint8_t);
void clearBoard(LedControl*, uint32_t[16]);
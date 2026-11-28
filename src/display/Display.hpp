#pragma once

#include <LedControl.h>
#include "physics/Physics.hpp"

extern uint32_t pause[16];
extern uint32_t loss[16];
extern uint32_t score[16];

bool compareCoordinates(Coordinate* coord1, Coordinate* coord2);
void flushBuffer(LedControl lcs[2], uint32_t board[16]);
void setPixel(uint32_t board[16], const Coordinate* coord, bool on);
bool getPixel(uint32_t board[16], const Coordinate* coord);
bool rowFullContains(uint32_t board[16], uint8_t rowNum);
void setRow(LedControl lcs[2], uint32_t board[16], uint8_t rowNum, bool on);
bool rowContains(uint32_t board[16], uint8_t rowNum, bool status);
void moveRow(uint32_t board[16], uint8_t curRow, uint8_t destRow);
void clearBoard(LedControl lcs[2], uint32_t board[16]);
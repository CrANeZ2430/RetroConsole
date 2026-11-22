#pragma once

#include <Arduino.h>
#include <LedControl.h>
#include "../display/Display.hpp"

typedef struct{
  int8_t dx;
  int8_t dy;
} Direction;

typedef struct{
  Coordinate* pieces;
  uint8_t count;
} Obstacle;

void drawObstacle(LedControl*, uint32_t[16], const Obstacle*);
bool isOwnPiece(const Obstacle*, const Coordinate*);
bool canMoveObstacle(uint32_t[16], const Obstacle*, const Direction*);
void moveObstacle(LedControl*, uint32_t[16], const Direction*, Obstacle*);
void initRandomObstacle(Obstacle*, Coordinate[4]);
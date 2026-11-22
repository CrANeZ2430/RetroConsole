#pragma once

#include <Arduino.h>
#include <LedControl.h>
#include "../display/Display.hpp"

struct Direction{
    int8_t dx;
    int8_t dy;
};

struct Obstacle{
    Coordinate* pieces;
    uint8_t count;
};

void drawObstacle(LedControl*, uint32_t[16], const Obstacle*);
bool isOwnPiece(const Obstacle*, const Coordinate*);
bool canMoveObstacleX(uint32_t[16], const Obstacle*, const Direction*, bool);
bool canMoveObstacleY(uint32_t[16], const Obstacle*, const Direction*, bool);
void moveObstacle(LedControl*, uint32_t[16], const Direction*, Obstacle*, bool);
void initRandomObstacle(Obstacle*, Coordinate[4]);
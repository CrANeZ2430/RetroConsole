#pragma once

#include <LedControl.h>
#include "physics/Physics.hpp"

struct Obstacle{
    Coordinate* pieces;
    uint8_t count;
};

void drawObstacle(LedControl*, uint32_t[16], Obstacle*);
bool isOwnPiece(const Obstacle *, const Coordinate *);
bool canMoveObstacleX(uint32_t[16], Obstacle*, Direction*, bool);
bool canMoveObstacleY(uint32_t[16], Obstacle*, Direction*, bool);
bool moveObstacle(LedControl*, uint32_t[16], const Direction*, const Obstacle*, bool);
void spawnObstacle(LedControl*, uint32_t[16], Obstacle*, Coordinate*,
    uint8_t, uint8_t, uint8_t, uint8_t);
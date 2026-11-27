#pragma once

#include <LedControl.h>
#include "physics/Physics.hpp"

struct Obstacle{
    Coordinate pieces[5];
    uint8_t count;
};

void drawObstacle(LedControl*, uint32_t[16], Obstacle*);
bool isOwnPiece(const Obstacle *, const Coordinate *);
bool canMoveObstacleX(uint32_t[16], Obstacle*, Direction*, bool);
bool canMoveObstacleY(uint32_t[16], Obstacle*, Direction*, bool);
bool moveObstacle(LedControl*, uint32_t[16], const Direction*, Obstacle*, bool);
void initObstacle(Obstacle*);
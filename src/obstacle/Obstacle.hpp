#pragma once

#include <LedControl.h>
#include "physics/Physics.hpp"

struct Obstacle{
    Coordinate pieces[5];
    uint8_t count;
};

void drawObstacle(LedControl lcs[2], uint32_t board[16], Obstacle* obst);
// bool isOwnPiece(const Obstacle* obst, const Coordinate * target);
// bool canMoveObstacleX(uint32_t board[16], Obstacle* obst, Direction* dir, bool spawning);
// bool canMoveObstacleY(uint32_t board[16], Obstacle* obst, Direction* dir, bool spawning);
bool moveObstacle(LedControl lcs[2], uint32_t board[16], const Direction* dir, Obstacle* obst, bool spawning);
void initObstacle(Obstacle* obst);
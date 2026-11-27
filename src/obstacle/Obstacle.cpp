#include "Obstacle.hpp"
#include "../display/Display.hpp"
#include "../joystick/Joystick.hpp"

static constexpr uint8_t maxX = 15;
static constexpr uint8_t maxY = 31;

void drawObstacle(LedControl lcs[2], uint32_t board[16], const Obstacle* obst){
    for (size_t i = 0; i < obst->count; ++i) {
        setPixel(board, &obst->pieces[i], true);
    }

    flushBuffer(lcs, board);
}

bool isOwnPiece(const Obstacle *obst, const Coordinate* target) {
    for (uint8_t j = 0; j < obst->count; ++j) {
        if (compareCoordinates(target, &obst->pieces[j])) {
            return true;
        }
    }
    return false;
}

bool canMoveObstacleX(uint32_t board[16], const Obstacle* obst, const Direction* dir, bool isSpawning){
    for (uint8_t i = 0; i < obst->count; ++i) {
        const int nextX = obst->pieces[i].x + dir->dx;
        const int nextY = obst->pieces[i].y + dir->dy;

        if (((nextY < 0 || nextY > maxY) && !isSpawning) || (nextX < 0 || nextX > maxX)) {
        return false;
        }

        Coordinate nextCoord{static_cast<uint8_t>(nextX), static_cast<uint8_t>(nextY)};
        if (!isOwnPiece(obst, &nextCoord) && getPixel(board, &nextCoord)) {
        return false;
        }
    }
    
    return true;
}

bool canMoveObstacleY(uint32_t board[16], const Obstacle* obst, const Direction* dir, bool spawning){
    for (uint8_t i = 0; i < obst->count; ++i) {
        int nextY = obst->pieces[i].y + dir->dy;

        if ((nextY < 0 || nextY > maxY) && !spawning) {
        return false;
        }

        Coordinate nextCoord{obst->pieces[i].x, static_cast<uint8_t>(nextY)};
        if (!isOwnPiece(obst, &nextCoord) && getPixel(board, &nextCoord)) {
        return false;
        }
    }
    
    return true;
}

bool moveObstacle(LedControl lcs[2], uint32_t board[16], const Direction* dir, Obstacle* obst, bool spawning){
    uint8_t dx = dir->dx;
    const uint8_t dy = dir->dy;
        
    if (!canMoveObstacleY(board, obst, dir, spawning)){
        return true;
    }

    if (!canMoveObstacleX(board, obst, dir, spawning)){
        dx = 0;
    }

    for (uint8_t i = 0; i < obst->count; ++i){
        setPixel(board, &obst->pieces[i], false);
    }

    for (uint8_t i = 0; i < obst->count; ++i){
        if (dx != 0)
        obst->pieces[i].x += dx;
        if (dy != 0)
        obst->pieces[i].y += dy;
            
        setPixel(board, &obst->pieces[i], true);
    }

    flushBuffer(lcs, board);
    return false;
}

void initObstacle(Obstacle* obst) {
    if (!obst || !obst->pieces) {
        return;
    }

    const uint8_t obstacleNumber = static_cast<uint8_t>(random(0, 6));

    switch (obstacleNumber) {
        case 0: {
            const uint8_t randomX = static_cast<uint8_t>(random(0, 15));
            obst->pieces[0] = { randomX, 34 };
            obst->pieces[1] = { randomX, 33 };
            obst->pieces[2] = { randomX, 32 };
            obst->pieces[3] = { static_cast<uint8_t>(randomX + 1), 32 };
            obst->count = 4;
            break;
        }
        case 1: {
            const uint8_t randomX = static_cast<uint8_t>(random(0, 15));
            obst->pieces[0] = { randomX, 32 };
            obst->pieces[1] = { static_cast<uint8_t>(randomX + 1), 32 };
            obst->pieces[2] = { static_cast<uint8_t>(randomX + 1), 33 };
            obst->pieces[3] = { static_cast<uint8_t>(randomX + 1), 34 };
            obst->count = 4;
            break;
        }
        case 2: {
            const uint8_t randomX = static_cast<uint8_t>(random(0, 15));
            obst->pieces[0] = { randomX, 32 };
            obst->pieces[1] = { randomX, 33 };
            obst->pieces[2] = { static_cast<uint8_t>(randomX + 1), 32 };
            obst->pieces[3] = { static_cast<uint8_t>(randomX + 1), 33 };
            obst->count = 4;
            break;
        }
        case 3: {
            const uint8_t randomX = static_cast<uint8_t>(random(1, 14));
            obst->pieces[0] = { static_cast<uint8_t>(randomX - 1), 34 };
            obst->pieces[1] = { randomX, 34 };
            obst->pieces[2] = { randomX, 33 };
            obst->pieces[3] = { randomX, 32 };
            obst->pieces[4] = { static_cast<uint8_t>(randomX + 1), 34 };
            obst->count = 5;
            break;
        }
        case 4: {
            const uint8_t randomX = static_cast<uint8_t>(random(1, 14));
            obst->pieces[0] = {static_cast<uint8_t>(randomX - 1), 32};
            obst->pieces[1] = {randomX, 32};
            obst->pieces[2] = {randomX, 33};
            obst->pieces[3] = {static_cast<uint8_t>(randomX + 1), 33};
            obst->count = 4;
            break;
        }
        case 5: {
            const uint8_t randomX = static_cast<uint8_t>(random(1, 14));
            obst->pieces[0] = {static_cast<uint8_t>(randomX - 1), 33};
            obst->pieces[1] = {randomX, 33};
            obst->pieces[2] = {randomX, 32};
            obst->pieces[3] = {static_cast<uint8_t>(randomX + 1), 32};
            obst->count = 4;
            break;
        }
    }
}
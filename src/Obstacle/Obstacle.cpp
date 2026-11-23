#include "Obstacle.hpp"

static constexpr uint8_t maxX = 15;
static constexpr uint8_t maxY = 31;

void drawObstacle(LedControl* lc, uint32_t board[16], const Obstacle* obstacle){
    for (size_t i = 0; i < obstacle->count; ++i) {
        setPixel(board, &obstacle->pieces[i], true);
    }

    flushBuffer(board, lc);
}

bool isOwnPiece(const Obstacle* obstacle, const Coordinate* target) {
    for (uint8_t j = 0; j < obstacle->count; ++j) {
        if (compareCoordinates(target, &obstacle->pieces[j])) {
        return true;
        }
    }
    return false;
}

bool canMoveObstacleX(uint32_t board[16], const Obstacle* obstacle, const Direction* direction, bool isSpawning){
    for (uint8_t i = 0; i < obstacle->count; ++i) {
        int nextX = obstacle->pieces[i].x + direction->dx;
        int nextY = obstacle->pieces[i].y + direction->dy;

        if (((nextY < 0 || nextY > maxY) && !isSpawning) || (nextX < 0 || nextX > maxX)) {
        return false;
        }

        Coordinate nextCoord{static_cast<uint8_t>(nextX), static_cast<uint8_t>(nextY)};
        if (!isOwnPiece(obstacle, &nextCoord) && getPixel(board, &nextCoord)) {
        return false;
        }
    }
    
    return true;
}

bool canMoveObstacleY(uint32_t board[16], const Obstacle* obstacle, const Direction* direction, bool isSpawning){
    for (uint8_t i = 0; i < obstacle->count; ++i) {
        int nextY = obstacle->pieces[i].y + direction->dy;

        if ((nextY < 0 || nextY > maxY) && !isSpawning) {
        return false;
        }

        Coordinate nextCoord{obstacle->pieces[i].x, static_cast<uint8_t>(nextY)};
        if (!isOwnPiece(obstacle, &nextCoord) && getPixel(board, &nextCoord)) {
        return false;
        }
    }
    
    return true;
}

bool moveObstacle(LedControl* lc, uint32_t board[16], const Direction* direction, Obstacle* obstacle, bool isSpawning){
    uint8_t dx = direction->dx;
    uint8_t dy = direction->dy;
        
    if (!canMoveObstacleY(board, obstacle, direction, isSpawning)){
        return true;
    }

    if (!canMoveObstacleX(board, obstacle, direction, isSpawning)){
        dx = 0;
    }

    for (uint8_t i = 0; i < obstacle->count; ++i){
        setPixel(board, &obstacle->pieces[i], false);
    }

    for (uint8_t i = 0; i < obstacle->count; ++i){
        if (dx != 0)
        obstacle->pieces[i].x += dx;
        if (dy != 0)
        obstacle->pieces[i].y += dy;
            
        setPixel(board, &obstacle->pieces[i], true);
    }

    flushBuffer(board, lc);
    return false;
}
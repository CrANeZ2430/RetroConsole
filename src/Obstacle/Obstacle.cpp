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

bool canMoveObstacle(uint32_t board[16], const Obstacle* obstacle, const Direction* direction){
  for (uint8_t i = 0; i < obstacle->count; ++i) {
    int nextX = obstacle->pieces[i].x + direction->dx;
    int nextY = obstacle->pieces[i].y + direction->dy;

    if (nextX < 0 || nextX > maxX || nextY < 0) {
      return false;
    }

    Coordinate nextCoord{static_cast<uint8_t>(nextX), static_cast<uint8_t>(nextY)};
    if (!isOwnPiece(obstacle, &nextCoord) && getPixel(board, &nextCoord)) {
      return false;
    }
  }
  
  return true;
}

void moveObstacle(LedControl* lc, uint32_t board[16], const Direction* direction, Obstacle* obstacle){
  if (!canMoveObstacle(board, obstacle, direction)){
    return;
  }

  for (uint8_t i = 0; i < obstacle->count; ++i){
    setPixel(board, &obstacle->pieces[i], false);
  }

  for (uint8_t i = 0; i < obstacle->count; ++i){
    if (direction->dx != 0)
      obstacle->pieces[i].x += direction->dx;
    if (direction->dy != 0)
      obstacle->pieces[i].y += direction->dy;
    
    setPixel(board, &obstacle->pieces[i], true);
  }

  flushBuffer(board, lc);
}

void initRandomObstacle(Obstacle* outObstacle, Coordinate outPieces[4]){
  uint8_t x = static_cast<uint8_t>(random(0, 15));
  outPieces[0] = {x, 34};
  outPieces[1] = {x, 33};
  outPieces[2] = {x, 32};
  outPieces[3] = {static_cast<uint8_t>(x + 1), 32};

  outObstacle->pieces = outPieces;
  outObstacle->count = 4;
}
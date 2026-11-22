#include <Arduino.h>
#include <LedControl.h>
#include "./joystick/Joystick.hpp"
#include "./button/Button.hpp"

static constexpr uint8_t analogX = A0;
static constexpr uint8_t analogY = A1;
static constexpr uint8_t joystickButton = A2;
static constexpr uint8_t resetButton = A3;
static constexpr uint8_t menuButton = A4;

static constexpr uint8_t dataInput = 10;
static constexpr uint8_t chipSelect = 11;
static constexpr uint8_t clock = 13;

static constexpr int NUM_DEVICES = 8;
static LedControl lc = LedControl(11, 13, 10, NUM_DEVICES);

static uint32_t board[16] = {
  0b00000000000000000000000000011100,
  0b00000000000000000000000000000000,
  0b00000000000000000000000000000000,
  0b00000000000000000111000000000000,
  0b11100000000000000000000000000000,
  0b00000001100000000000000000000000,
  0b00000000000000000000001000000000,
  0b00000000000000000000001000000000,
  0b00000000000000000000001000000000,
  0b00000000000000000000011000000000,
  0b00000000000000000000000000000000,
  0b00000000000000011000000000000000,
  0b00000000000000000000000000000000,
  0b00000000000000000000000000001111,
  0b00000000000000000000000000000111,
  0b00000000000000000000000000000000
};

static constexpr uint8_t maxX = 15;
static constexpr uint8_t maxY = 31;

typedef struct{
  int8_t dx;
  int8_t dy;
} Direction;

typedef struct{
  uint8_t x;
  uint8_t y;
} Coordinate;

typedef struct{
  Coordinate* pieces;
  uint8_t count;
} Obstacle;

void flushBuffer(uint32_t[16]);
uint8_t reverseBuffer(uint8_t);
void setPixel(Coordinate*, bool);
bool getPixel(Coordinate*);

bool compareCoordinates(Coordinate* c1, Coordinate* c2){
  if (!c1 || !c2) {
    return false;
  }
  return c1->x == c2->x && c1->y == c2->y;
}

Direction getDirection(){
  JoystickState state = readJoystick(analogX, analogY, joystickButton);
  if (state.x > 600){
    return {1, 0};
  }
  else if (state.x < 100){
    return {-1, 0};
  }
  else{
    return {0, 0};
  }
}

void drawObstacle(const Obstacle* obstacle){
  for (size_t i = 0; i < obstacle->count; ++i) {
    setPixel(&obstacle->pieces[i], true);
  }

  flushBuffer(board);
}

bool isOwnPiece(const Obstacle* obstacle, const Coordinate* target) {
  for (uint8_t j = 0; j < obstacle->count; ++j) {
    if (compareCoordinates(target, &obstacle->pieces[j])) {
      return true;
    }
  }
  return false;
}

bool canMoveObstacle(Obstacle* obstacle, Direction* direction){
  for (uint8_t i = 0; i < obstacle->count; ++i) {
    int nextX = obstacle->pieces[i].x + direction->dx;
    int nextY = obstacle->pieces[i].y + direction->dy;

    if (nextX < 0 || nextX > maxX || nextY < 0 || nextY > maxY) {
      return false;
    }

    Coordinate nextCoord{static_cast<uint8_t>(nextX), static_cast<uint8_t>(nextY)};
    if (!isOwnPiece(obstacle, &nextCoord) && getPixel(&nextCoord)) {
      return false;
    }
  }
  
  return true;
}

void moveObstacle(Direction* direction, Obstacle* obstacle){
  if (!canMoveObstacle(obstacle, direction)){
    return;
  }

  for (uint8_t i = 0; i < obstacle->count; ++i){
    setPixel(&obstacle->pieces[i], false);
  }

  for (uint8_t i = 0; i < obstacle->count; ++i){
    if (direction->dx != 0)
      obstacle->pieces[i].x += direction->dx;
    if (direction->dy != 0)
      obstacle->pieces[i].y += direction->dy;
    
    setPixel(&obstacle->pieces[i], true);
  }

  flushBuffer(board);
}

// void spawnObstacle(const Coordinate spawnPoints[2], uint8_t height){

// }

void setPixel(Coordinate* coordinate, bool on){
  if (coordinate->x < 0 || coordinate->x > maxX || 
    coordinate->y < 0 || coordinate->y > maxY){
    return;
  }

  uint32_t row = 1UL << (coordinate->y);
  if (on){
    board[15-coordinate->x] |= row;
  }
  else{
    board[15-coordinate->x] &= ~row;
  }
}

bool getPixel(Coordinate* coordinate){
  if (coordinate->x < 0 || coordinate->x > maxX || 
    coordinate->y < 0 || coordinate->y > maxY){
    return false;
  }

  if ((board[15-coordinate->x] & (1UL << coordinate->y)) != 0){
    return true;
  }
  else{
    return false;
  }
}

Coordinate pieces[] = {{0, 31}, {0, 30}, {0, 29}, {1, 29}};
Obstacle obstacle{pieces, 4};

void setup() {

  for (int i = 0; i < NUM_DEVICES; ++i) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 4);
    lc.clearDisplay(i);
  }

  drawObstacle(&obstacle);
}

void loop() {
  //Direction dir = getDirection();
  Direction down = {0, -1};
  moveObstacle(&down, &obstacle);
  delay(250);
}

void flushBuffer(uint32_t data[16]){
  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row] >> 24;
    buffer = reverseBuffer(buffer);
    lc.setRow(3, row, buffer);
  }

  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row] >> 16;
    buffer = reverseBuffer(buffer);
    lc.setRow(2, row, buffer);
  }

  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row] >> 8;
    buffer = reverseBuffer(buffer);
    lc.setRow(1, row, buffer);
  }

  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row] >> 0;
    buffer = reverseBuffer(buffer);
    lc.setRow(0, row, buffer);
  }

  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row+8] >> 24;
    lc.setRow(4, 7-row, buffer);
  }

  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row+8] >> 16;
    lc.setRow(5, 7-row, buffer);
  }

  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row+8] >> 8;
    lc.setRow(6, 7-row, buffer);
  }

  for (int row = 0; row < 8; row++){
    uint8_t buffer = data[row+8] >> 0;
    lc.setRow(7, 7-row, buffer);
  }
}

uint8_t reverseBuffer(uint8_t b){
  uint8_t rev = 0;
  for (uint8_t i = 0; i < 8; i++) {
      rev = (rev << 1) | (b & 1u);
      b >>= 1;
  }
  return rev;
}
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

const int NUM_DEVICES = 8;
LedControl lc = LedControl(11, 13, 10, NUM_DEVICES);

uint32_t board[16] = {
  0b11100000000000000000000000011100,
  0b00100000000000000000000000000000,
  0b00000000000000000000000000000000,
  0b00000000000000000111000000000000,
  0b00000000000000000000000000000000,
  0b00000001100000000000000000000000,
  0b00000000000000000000001000000000,
  0b00000000000000000000001000000000,
  0b00000000000000000000001000000000,
  0b00000000000000000000011000000000,
  0b00000000000000000000000000000000,
  0b00000000000000011000000000000000,
  0b00000000000000000000000000000000,
  0b00000000000000000000000000001111,
  0b00000000000000000000000000000000,
  0b00000000000000000000000000000000
};

//uint32_t frame[16];

typedef struct{
  bool right;
  bool left;
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

Direction readMovement(){
  JoystickState state = readJoystick(analogX, analogY, joystickButton);
  Serial.println(state.x);
  if (state.x > 600){
    return {false, true};
  }
  else if (state.x < 100) {
    return {true, false};
  }
  else {
    return {false, false};
  }
}

void drawObstacle(const Obstacle* obstacle){
  for (size_t i = 0; i < obstacle->count; ++i) {
    setPixel(&obstacle->pieces[i], true);
  }

  flushBuffer(board);
}

void moveObstacle(Direction* direction, Obstacle* obstacle){
  uint8_t dx = 0;
  if (direction->left){
    dx = -1;
  }
  else if (direction-> right){
    dx = 1;
  }

  for (uint8_t i = 0; i < obstacle->count; ++i){
    setPixel(&obstacle->pieces[i], false);
  }

  for (uint8_t i = 0; i < obstacle->count; ++i){
    obstacle->pieces[i].x += dx;
    setPixel(&obstacle->pieces[i], true);
  }

  flushBuffer(board);
}

void setPixel(Coordinate* coordinate, bool on){
  uint32_t row = 1UL << (coordinate->y);
  if (on){
    board[15-(coordinate->x)] |= row;
  }
  else{
    board[15-(coordinate->x)] &= ~row;
  }
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_DEVICES; ++i) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 4);
    lc.clearDisplay(i);
  }

  Coordinate pieces[] = {{0, 31}, {0, 30}, {0, 29}, {1, 29}};
  Direction direction{true, false};
  Obstacle obstacle{pieces, 4};
  drawObstacle(&obstacle);

  delay(200);
  moveObstacle(&direction, &obstacle);
}

void loop() {
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
#include <Arduino.h>
#include <LedControl.h>
#include "./joystick/Joystick.hpp"
#include "./button/Button.hpp"
#include "./Obstacle/Obstacle.hpp"

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
  0b00000000000000000000000000000111,
  0b00000000000000000000000000000000
};

static constexpr uint8_t maxX = 15;
static constexpr uint8_t maxY = 31;

Direction getDirection(){
  JoystickState state = readJoystick(analogX, analogY, joystickButton);
  if (state.x > 600){
    return {-1, 0};
  }
  else if (state.x < 100){
    return {1, 0};
  }
  else{
    return {0, 0};
  }
}

void spawnObstacle(Obstacle* obstacle, uint8_t height){
  for (uint8_t i = 0; i < height; ++i){
      Direction dir = getDirection();
      Direction down = {dir.dx, -1};
      moveObstacle(&lc, board, &down, obstacle, true);
      delay(250);
  }
}

static Obstacle obstacle;
static Coordinate coordinates[4];

void setup() {
  for (int i = 0; i < NUM_DEVICES; ++i) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 4);
    lc.clearDisplay(i);
  }

  initRandomObstacle(&obstacle, coordinates);
  flushBuffer(board, &lc);
  delay(250);
  spawnObstacle(&obstacle, 3);
}

void loop() {
  Direction dir = getDirection();
  Direction down = {dir.dx, -1};
  moveObstacle(&lc, board, &down, &obstacle, false);
  delay(250);
}
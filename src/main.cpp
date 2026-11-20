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

uint32_t data[16] = {
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

void flushBuffer(uint32_t[]);
uint8_t reverseBuffer(uint8_t);

void setup() {
  // lc.shutdown(4, false);
  // lc.setIntensity(4, 4);
  // lc.clearDisplay(4);

  // for (int row = 0; row < 8; row++){
  //   lc.setRow(4, row, data[row]);
  // }

  // delay(500);
  flushBuffer(data);
}

void loop() {
  // data[0] = data[0] >> 1;
  // data[1] = data[1] >> 1;

  // for (int row = 0; row < 8; row++){
  //   lc.setRow(4, row, data[row]);
  // }

  // delay(500);
}

void flushBuffer(uint32_t data[]){
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
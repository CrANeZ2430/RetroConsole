#include <Arduino.h>
#include "./joystick/Joystick.hpp"
#include "./button/Button.hpp"

static constexpr uint8_t analogX = A0;
static constexpr uint8_t analogY = A1;
static constexpr uint8_t button = A2;
static constexpr uint8_t menuButton = A3;
static constexpr uint8_t resetButton = A4;

void printJoystickState();

void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(menuButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  //printJoystickState();
  int val = readButton(menuButton);
  Serial.println(val);
  delay(1000);
}

void printJoystickState(){
  auto state = readJoystick(analogX, analogY, button);
  Serial.print(state.x);
  Serial.print(" | ");
  Serial.print(state.y);
  Serial.print(" | ");
  Serial.print(state.pressed);
  Serial.println();
}
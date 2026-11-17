#include <Arduino.h>

static constexpr uint8_t analogX = A0;
static constexpr uint8_t analogY = A1;
static constexpr uint8_t button = A2;

typedef struct{
  int x;
  int y;
  bool pressed;
} joystickState;

int readChannel(uint8_t pin);
joystickState readJoystick();

void setup() {
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  auto state = readJoystick();
  Serial.print(state.x);
  Serial.print(" | ");
  Serial.print(state.y);
  Serial.print(" | ");
  Serial.print(state.pressed);
  Serial.println();
  delay(1000);
}

int readChannel(uint8_t pin) {
    analogRead(pin);
    return analogRead(pin);
}

joystickState readJoystick(){
  int x = readChannel(analogX);
  int y = readChannel(analogY);
  int pressed = digitalRead(button);

  Serial.println(x);
  Serial.println(y);
  Serial.println(pressed);

  return {x, y, pressed == 0 ? false : true};
}
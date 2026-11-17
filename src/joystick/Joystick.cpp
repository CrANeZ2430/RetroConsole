#include "Joystick.hpp"

joystickState readJoystick(uint8_t pinX, uint8_t pinY, uint8_t pinButton){
  int x = analogRead(pinX);
  int y = analogRead(pinY);
  int pressed = digitalRead(pinButton);

  return {x, y, pressed == 0 ? false : true};
}
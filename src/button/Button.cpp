#include "Button.hpp"

int readButton(uint8_t pinButton){
  return digitalRead(pinButton);
}
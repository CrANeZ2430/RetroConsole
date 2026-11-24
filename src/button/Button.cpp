#include "Button.hpp"

bool readButton(uint8_t pinButton){
    return digitalRead(pinButton);
}
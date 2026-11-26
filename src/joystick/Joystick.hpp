#pragma once

#include <Arduino.h>
#include "physics/Physics.hpp"

struct JoystickState{
    int x;
    int y;
    bool pressed;
};

JoystickState readJoystick(uint8_t pinX, uint8_t pinY, uint8_t pinButton);
Direction getDirection(uint8_t pinX, uint8_t pinY, uint8_t pinButton);
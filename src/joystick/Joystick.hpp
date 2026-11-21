#pragma once

#include <Arduino.h>

typedef struct {
    int x;
    int y;
    bool pressed;
} JoystickState;

JoystickState readJoystick(uint8_t pinX, uint8_t pinY, uint8_t pinButton);
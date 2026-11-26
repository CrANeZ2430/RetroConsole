#include "Joystick.hpp"

JoystickState readJoystick(const uint8_t pinX, const uint8_t pinY, const uint8_t pinButton){
    const int x = analogRead(pinX);
    const int y = analogRead(pinY);
    const int pressed = digitalRead(pinButton);

    return {x, y, pressed != LOW};
}

Direction getDirection(const uint8_t pinX, const uint8_t pinY, const uint8_t pinButton){
    JoystickState state = readJoystick(pinX, pinY, pinButton);
    Direction dir = {0, 0};

    if (state.x > 600){
        dir.dx = -1;
    }
    else if (state.x < 100){
        dir.dx = 1;
    }

    if (state.y < 100){
        dir.dy = -1;
    }
    else if (state.y > 600){
        dir.dy = 1;
    }

    return dir;
}
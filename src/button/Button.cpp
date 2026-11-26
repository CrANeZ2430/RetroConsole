#include "Button.hpp"

static constexpr uint32_t DEBOUNCE_MS = 50;
static uint32_t lastMenuChangeMs = 0;
static bool prevMenuDown = false;

bool readButton(const uint8_t pinButton){
    return digitalRead(pinButton);
}

bool buttonJustPressed(const uint8_t pinButton) {
    bool down = !readButton(pinButton);
    unsigned long now = millis();
    bool triggered = false;

    if (down != prevMenuDown && (now - lastMenuChangeMs) >= DEBOUNCE_MS) {
        lastMenuChangeMs = now;
        prevMenuDown = down;
        if (down) {
            triggered = true;
        }
    }
    return triggered;
}
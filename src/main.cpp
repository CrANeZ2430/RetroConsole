#include <Arduino.h>
#include <LedControl.h>
#include "button/Button.hpp"
#include "joystick/Joystick.hpp"
#include "display/Display.hpp"
#include "obstacle/Obstacle.hpp"
#include "gameState/GameState.hpp"

static constexpr uint8_t joystickX = A0;
static constexpr uint8_t joystickY = A1;
static constexpr uint8_t joystickButton = A2;
static constexpr uint8_t menuButton = A3;
//static constexpr uint8_t pauseButton = A4;

static constexpr uint8_t dataInput = 11;
static constexpr uint8_t chipSelect = 10;
static constexpr uint8_t clock = 13;

static constexpr uint8_t NUM_DEVICES = 8;
static LedControl lc = LedControl(dataInput, clock, chipSelect, NUM_DEVICES);

static uint32_t board[16] = {
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL
};

static uint32_t lastMenuChangeMs = 0;
static uint8_t rowsCleared = 0;
static Obstacle currentObstacle = { {}, 0};
static GameState gameState = Playing;
static bool buttonPressed = false;

void setup() {
    randomSeed(analogRead(A5));
    for (int i = 0; i < NUM_DEVICES; ++i) {
        lc.shutdown(i, false);
        lc.setIntensity(i, 4);
        lc.clearDisplay(i);
    }

    pinMode(menuButton, INPUT_PULLUP);

    initObstacle(&currentObstacle);
    flushBuffer(&lc, board);
}

void loop() {
    if (gameState == Playing) {
        Direction direction = getDirection(joystickX, joystickY, joystickButton);
        direction.dy = -1;
        moveObstacle(&lc, board, &direction, &currentObstacle, true);

        delay(50);
    }
}
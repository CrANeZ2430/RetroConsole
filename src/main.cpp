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

static constexpr uint8_t dataInput1 = 11;
static constexpr uint8_t chipSelect1 = 10;
static constexpr uint8_t dataInput2 = 9;
static constexpr uint8_t chipSelect2 = 8;
static constexpr uint8_t clock = 13;

static constexpr uint8_t NUM_DEVICES = 4;
static constexpr uint8_t LC_COUNT = 2;


static LedControl lcs[LC_COUNT] = {
    LedControl(dataInput1, clock, chipSelect1, NUM_DEVICES),
    LedControl(dataInput2, clock, chipSelect2, NUM_DEVICES)
};

static uint32_t board[16] = {
    0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL
};

static uint32_t lastMenuChangeMs = 0;
static uint8_t rowsCleared = 0;
static Obstacle currentObstacle = { {}, 0};
static GameState gameState = Playing;
static bool buttonPressed = false;

void setup() {
    randomSeed(analogRead(A5));

    for (int lcNum = 0; lcNum < LC_COUNT; ++lcNum) {
        for (int displayNum = 0; displayNum < NUM_DEVICES; ++displayNum) {
            lcs[lcNum].shutdown(displayNum, false);
            lcs[lcNum].setIntensity(displayNum, 4);
            lcs[lcNum].clearDisplay(displayNum);
        }
    }

    pinMode(menuButton, INPUT_PULLUP);

    initObstacle(&currentObstacle);
    flushBuffer(lcs, board);
}

void loop() {
    if (gameState == Playing) {
        Direction direction = getDirection(joystickX, joystickY, joystickButton);
        direction.dy = -1;
        moveObstacle(lcs, board, &direction, &currentObstacle, true);

        delay(50);
    }
}
#include <Arduino.h>
#include <LedControl.h>
#include "button/Button.hpp"
#include "joystick/Joystick.hpp"
#include "display/Display.hpp"
#include "obstacle/Obstacle.hpp"
#include "gameState/GameState.hpp"

//static constexpr uint8_t maxX = 15;
static constexpr uint8_t MAX_Y = 31;

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
    0UL, 0UL, 0UL, 0UL,
};

static uint32_t lastMenuChangeMs = 0;
static bool buttonPressed = false;
static uint8_t rowsCleared = 0;
static GameState gameState = Playing;
static Obstacle curObst = { {}, 0};

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

    initObstacle(&curObst);
    flushBuffer(lcs, board);
}

static uint8_t delayTime = 25;
void loop() {
    if (gameState == Playing) {
        Direction dir = getDirection(joystickX, joystickY, joystickButton);
        delayTime = dir.dy == -1 ? 25 : 75;
        dir.dy = -1;
        const bool canMove = moveObstacle(lcs, board, &dir, &curObst, true);

        if (!canMove) {
            // if (rowFullContains(board, 31)) {
            //     gameState = Lost;
            //     flushBuffer(lcs, loss);
            //     return;
            // }

            for (uint8_t rowNum1 = 0; rowNum1 < MAX_Y; ++rowNum1) {
                const bool winRow = rowFullContains(board, rowNum1);

                if (winRow) {
                    setRow(lcs, board, rowNum1, false);
                    flushBuffer(lcs, board);
                    delay(delayTime);
                    setRow(lcs, board, rowNum1, true);
                    flushBuffer(lcs, board);
                    delay(delayTime);
                    setRow(lcs, board, rowNum1, false);
                    flushBuffer(lcs, board);
                    for (uint8_t rowNum2 = rowNum1 + 1; rowNum2 <= MAX_Y; ++rowNum2) {
                        if (rowFullContains(board, rowNum2)) {
                            continue;
                        }

                        moveRow(board, rowNum2, rowNum2 - 1);
                    }
                    flushBuffer(lcs, board);
                }
            }

            initObstacle(&curObst);
        }

        delay(delayTime);
    }
    else if (gameState == Paused) {}
    else if (gameState == Lost) {}
}
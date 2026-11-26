#include <Arduino.h>
#include <LedControl.h>
#include "button/Button.hpp"
#include "joystick/Joystick.hpp"
#include "display/Display.hpp"
#include "obstacle/Obstacle.hpp"

static constexpr uint8_t analogX = A0;
static constexpr uint8_t analogY = A1;
static constexpr uint8_t joystickButton = A2;
static constexpr uint8_t menuButton = A3;
//static constexpr uint8_t pauseButton = A4;

static constexpr uint8_t dataInput = 11;
static constexpr uint8_t chipSelect = 10;
static constexpr uint8_t clock = 13;

static constexpr uint8_t NUM_DEVICES = 8;
static LedControl lc = LedControl(dataInput, clock, chipSelect, NUM_DEVICES);

static uint32_t lastMenuChangeMs = 0;

static uint32_t board[16] = {
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL
};

static uint8_t rowsCleared = 0;
static Obstacle obstacle;
static Coordinate coordinates[4];
static bool obstacleDown = true;
static int8_t gameState = 1;
static bool buttonPressed = false;

void setup() {
    for (int i = 0; i < NUM_DEVICES; ++i) {
        lc.shutdown(i, false);
        lc.setIntensity(i, 4);
        lc.clearDisplay(i);
    }

    pinMode(menuButton, INPUT_PULLUP);
    flushBuffer(&lc, board);
}

void loop() {
    if (gameState == 1){
        buttonPressed = buttonJustPressed(menuButton);

        if (buttonPressed){
            gameState = 0;
            lastMenuChangeMs = millis();
            flushBuffer(&lc, pause);
            return;
        }

        if (obstacleDown){
            if (rowContains(board, 31, true))
            {
                flushBuffer(&lc, loss);
                gameState = -1;
                return;
            }

            for (uint8_t i = 0; i < 32; ++i){
                bool winRow = checkRowWin(board, i);
                if (winRow){
                    for (uint8_t j = 0; j < 2; j++){
                        delay(200);
                        setRow(&lc, board, i, false);
                        delay(200);
                        setRow(&lc, board, i, true);
                    }
                    delay(200);
                    setRow(&lc, board, i, false);

                    for (uint8_t j = i+1; j < 32; ++j){
                        moveRow(board, j, j-1);
                    }

                    rowsCleared++;
                }
            }
            delay(50);
            spawnObstacle(&lc, board, &obstacle, coordinates, 3, analogX, analogY, joystickButton);
            obstacleDown = false;
        }

        const Direction dir = getDirection(analogX, analogY, joystickButton);
        const Direction down = {dir.dx, -1};
        obstacleDown = moveObstacle(&lc, board, &down, &obstacle, false);
        delay(dir.dy == -1 ? 50 : 200);
    }
    else if (gameState == -1) {
        buttonPressed = !readButton(menuButton);

        if (buttonPressed){
            gameState = 1;
            clearBoard(&lc, board);
            flushBuffer(&lc, board);
            delay(100);
            return;
        }

        delay(50);
    }
    else if (gameState == 0){
        delay(100);
        const bool buttonPressed = buttonJustPressed(menuButton);
        if (buttonPressed){
            gameState = 1;
            lastMenuChangeMs = millis();
            flushBuffer(&lc, board);
        }
    }
}
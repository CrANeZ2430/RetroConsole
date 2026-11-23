#include <Arduino.h>
#include <LedControl.h>
#include "./joystick/Joystick.hpp"
#include "./button/Button.hpp"
#include "./Obstacle/Obstacle.hpp"

static constexpr uint8_t analogX = A0;
static constexpr uint8_t analogY = A1;
static constexpr uint8_t joystickButton = A2;
static constexpr uint8_t resetButton = A3;
static constexpr uint8_t menuButton = A4;

static constexpr uint8_t dataInput = 10;
static constexpr uint8_t chipSelect = 11;
static constexpr uint8_t clock = 13;

static constexpr uint8_t NUM_DEVICES = 8;
static LedControl lc = LedControl(11, 13, 10, NUM_DEVICES);

static uint32_t board[16] = {
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL
};

static uint8_t rowsCleared = 0;
static Obstacle obstacle;
static Coordinate coordinates[4];
static bool obstacleDown = true;

Direction getDirection();
void spawnObstacle(Obstacle* outObstacle, Coordinate* outPieces, uint8_t height);
bool checkRowWin(uint32_t[16], uint8_t);
void setRow(uint32_t[16], uint8_t, bool);
void moveRow(uint32_t board[16], uint8_t rowNum, uint8_t nextRowNum);

void setup() {
    for (int i = 0; i < NUM_DEVICES; ++i) {
        lc.shutdown(i, false);
        lc.setIntensity(i, 4);
        lc.clearDisplay(i);
    }

    flushBuffer(board, &lc);
}

void loop() {
    if (obstacleDown){
        for (uint8_t i = 0; i < 32; ++i){
            bool winRow = checkRowWin(board, i);
            if (winRow){
                for (uint8_t j = 0; j < 2; j++){
                    delay(250);
                    setRow(board, i, false);
                    delay(250);
                    setRow(board, i, true);
                }
                delay(250);
                setRow(board, i, false);

                for (uint8_t j = i+1; j < 32; ++j){
                    moveRow(board, j, j-1);
                }

                rowsCleared++;
            }
        }
        delay(50);
        spawnObstacle(&obstacle, coordinates, 3);
        obstacleDown = false;
    }
    
    Direction dir = getDirection();
    Direction down = {dir.dx, -1};
    obstacleDown = moveObstacle(&lc, board, &down, &obstacle, false);
    delay(dir.dy == -1 ? 50 : 250);
}

Direction getDirection(){
    JoystickState state = readJoystick(analogX, analogY, joystickButton);
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

void spawnObstacle(Obstacle* outObstacle, Coordinate* outPieces, uint8_t height){
    uint8_t x = static_cast<uint8_t>(random(0, 15));
        outPieces[0] = {x, 34};
        outPieces[1] = {x, 33};
        outPieces[2] = {x, 32};
        outPieces[3] = {static_cast<uint8_t>(x + 1), 32};

        outObstacle->pieces = outPieces;
        outObstacle->count = 4;

    for (uint8_t i = 0; i < height; ++i){
        Direction dir = getDirection();
        Direction down = {dir.dx, -1};
        moveObstacle(&lc, board, &down, outObstacle, true);
        delay(dir.dy == -1 ? 50 : 250);
    }
}

bool checkRowWin(uint32_t board[16], uint8_t row){
    if (row < 0 || row > 31){
        return false;
    }

    uint32_t win = 1UL << row;

    for (uint16_t i = 0; i < 16; ++i){
        win &= (board[i] << row);
    }

    return win;
}

void setRow(uint32_t board[16], uint8_t rowNum, bool on){
    if (rowNum < 0 || rowNum > 31){
        return;
    }

    for (uint8_t i = 0; i < 16; ++i){
        uint32_t row = 1UL << rowNum;

        if (on){
        board[i] |= row;
        }
        else{
            board[i] &= ~row;
        }
    }
    flushBuffer(board, &lc);
}

void moveRow(uint32_t board[16], uint8_t rowNum, uint8_t nextRowNum){
    for (uint8_t i = 0; i < 16; ++i){
        Coordinate coord = {i, rowNum};
        bool value = getPixel(board, &coord);
        setPixel(board, &coord, false);
        coord.y = nextRowNum;
        setPixel(board, &coord, value);
    }
}
#include <Arduino.h>
#include <LedControl.h>
#include "./joystick/Joystick.hpp"
#include "./Obstacle/Obstacle.hpp"
#include "button/Button.hpp"

static constexpr uint8_t analogX = A0;
static constexpr uint8_t analogY = A1;
static constexpr uint8_t joystickButton = A2;
static constexpr uint8_t menuButton = A3;
static constexpr uint8_t pauseButton = A4;

static constexpr uint8_t dataInput = 11;
static constexpr uint8_t chipSelect = 10;
static constexpr uint8_t clock = 13;

static constexpr uint8_t NUM_DEVICES = 8;
static LedControl lc = LedControl(dataInput, clock, chipSelect, NUM_DEVICES);

static uint32_t board[16] = {
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL
};

static uint32_t frame[16] = {
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL,
    0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL
};

static uint8_t rowsCleared = 0;
static Obstacle obstacle;
static Coordinate coordinates[4];
static bool obstacleDown = true;
static int8_t gameState = 1;
static bool buttonPressed = false;

static Direction getDirection();
static void spawnObstacle(Obstacle*, Coordinate*, uint8_t);
static bool checkRowWin(uint32_t[16], uint8_t);
static void setRow(uint32_t[16], uint8_t, bool);
static void moveRow(uint32_t[16], uint8_t, uint8_t);
static bool rowContains(uint32_t[16], uint8_t, bool);

void setup() {
    for (int i = 0; i < NUM_DEVICES; ++i) {
        lc.shutdown(i, false);
        lc.setIntensity(i, 4);
        lc.clearDisplay(i);
    }

    pinMode(menuButton, INPUT_PULLUP);
    flushBuffer(board, &lc);
}

void loop() {
    if (gameState == 1){
        buttonPressed = !readButton(menuButton);

        if (buttonPressed){
            gameState = 0;

            frame[0] = board[0];
            frame[1] = board[1];
            frame[2] = board[2];
            frame[3] = board[3];
            frame[4] = board[4];
            frame[5] = board[5];
            frame[6] = board[6];
            frame[7] = board[7];
            frame[8] = board[8];
            frame[9] = board[9];
            frame[10] = board[10];
            frame[11] = board[11];
            frame[12] = board[12];
            frame[13] = board[13];
            frame[14] = board[14];
            frame[15] = board[15];

            board[0] = 0b00000000000000000000000000000000;
            board[1] = 0b00000000000000000000000000000000;
            board[2] = 0b00000000000000000000000000000000;
            board[3] = 0b00000001000100000000000000000000;
            board[4] = 0b01111000000000000000000001111100;
            board[5] = 0b00000100111001111100000001010100;
            board[6] = 0b01111001000101000101000101111100;
            board[7] = 0b00000001111101111100000000000000;
            board[8] = 0b00111100000000000001010101111100;
            board[9] = 0b01010001010101000101010101010100;
            board[10] = 0b00111101111101111101111101111100;
            board[11] = 0b00000000000000000000000000000000;
            board[12] = 0b01110001011101011101101101111100;
            board[13] = 0b01010001010101010101110001010100;
            board[14] = 0b01111101110101110101111101111100;
            board[15] = 0b00000000000000000000000000000000;

            flushBuffer(board, &lc);
            return;
        }

        if (obstacleDown){
            if (rowContains(board, 31, true))
            {
                board[0] = 0b00000000000000000000000000000000;
                board[1] = 0b00111100011100000000000000000000;
                board[2] = 0b01000010000011000101001000000000;
                board[3] = 0b01000010000000100101001000110110;
                board[4] = 0b01000010000000100101001001001000;
                board[5] = 0b01000010000011000101001001001000;
                board[6] = 0b00111100011100000111110001111110;
                board[7] = 0b00000000000000000000000000000000;
                board[8] = 0b00000000000000000000000000000000;
                board[9] = 0b00001110000111100111111000000000;
                board[10] = 0b01001010001001000010000001010010;
                board[11] = 0b01000010010001000001000001010010;
                board[12] = 0b01000010010001000001000001010010;
                board[13] = 0b01000010001001000010000001010010;
                board[14] = 0b00111100000111100111111001111100;
                board[15] = 0b00000000000000000000000000000000;
                
                flushBuffer(board, &lc);
                gameState = -1;
                return;
            }

            for (uint8_t i = 0; i < 32; ++i){
                bool winRow = checkRowWin(board, i);
                if (winRow){
                    for (uint8_t j = 0; j < 2; j++){
                        delay(200);
                        setRow(board, i, false);
                        delay(200);
                        setRow(board, i, true);
                    }
                    delay(200);
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
        delay(dir.dy == -1 ? 50 : 200);
    }
    else if (gameState == -1) {
        buttonPressed = !readButton(menuButton);

        if (buttonPressed){
            gameState = 1;
            board[0] = 0UL;
            board[1] = 0UL;
            board[2] = 0UL;
            board[3] = 0UL;
            board[4] = 0UL;
            board[5] = 0UL;
            board[6] = 0UL;
            board[7] = 0UL;
            board[8] = 0UL;
            board[9] = 0UL;
            board[10] = 0UL;
            board[11] = 0UL;
            board[12] = 0UL;
            board[13] = 0UL;
            board[14] = 0UL;
            board[15] = 0UL;

            flushBuffer(board, &lc);
            delay(100);
            return;
        }
        
        delay(50);
    }
    else if (gameState == 0){
        buttonPressed = !readButton(menuButton);
        if (buttonPressed){
            gameState = 1;

            board[0] = frame[0];
            board[1] = frame[1];
            board[2] = frame[2];
            board[3] = frame[3];
            board[4] = frame[4];
            board[5] = frame[5];
            board[6] = frame[6];
            board[7] = frame[7];
            board[8] = frame[8];
            board[9] = frame[9];
            board[10] = frame[10];
            board[11] = frame[11];
            board[12] = frame[12];
            board[13] = frame[13];
            board[14] = frame[14];
            board[15] = frame[15];
            flushBuffer(board, &lc);
        }
    }
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

bool rowContains(uint32_t board[16], uint8_t rowNum, bool on){
    for (uint8_t i = 0; i < 16; ++i){
        Coordinate coord = {i, rowNum};
        bool value = getPixel(board, &coord);
        if (value == on){
            return true;
        }
    }

    return false;
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
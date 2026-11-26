#include "Display.hpp"

static constexpr uint8_t maxX = 15;
static constexpr uint8_t maxY = 31;

uint32_t pause[16] = {
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b01111000000000000000000001111100,
    0b00000100111001111100000001010100,
    0b01111001000101000101000101111100,
    0b00000001111101111100000000000000,
    0b00111100000000000001010101111100,
    0b01010001010101000101010101010100,
    0b00111101111101111101111101111100,
    0b00000000000000000000000000000000,
    0b01110001011101011101101101111100,
    0b01010001010101010101110001010100,
    0b01111101110101110101111101111100,
    0b00000000000000000000000000000000
};

uint32_t loss[16] = {
    0b00000000000000000000000000000000,
    0b00111100011100000000000000000000,
    0b01000010000011000101001000000000,
    0b01000010000000100101001000110110,
    0b01000010000000100101001001001000,
    0b01000010000011000101001001001000,
    0b00111100011100000111110001111110,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00001110000111100111111000000000,
    0b01001010001001000010000001010010,
    0b01000010010001000001000001010010,
    0b01000010010001000001000001010010,
    0b01000010001001000010000001010010,
    0b00111100000111100111111001111100,
    0b00000000000000000000000000000000
};

uint32_t score[16] = {
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000001111100000000000000,
    0b01111100000001010100000000000000,
    0b01000101000101111100000000000000,
    0b01111100000000000000000000000000,
    0b00000001010101111100000000000000,
    0b01000101010101010100000000000000,
    0b01111101111101111100000000000000,
    0b00000000000000000000000000000000,
    0b01011101101101111100000000000000,
    0b01010101110001010100000000000000,
    0b01110101111101111100000000000000,
    0b00000000000000000000000000000000
};

static uint8_t reverseBuffer(uint8_t b){
    uint8_t rev = 0;
    for (uint8_t i = 0; i < 8; i++) {
        rev = (rev << 1) | (b & 1u);
        b >>= 1;
    }
    return rev;
}

void flushBuffer(LedControl* lc, uint32_t data[16]){
    for (int row = 0; row < 8; row++){
        uint8_t buffer = data[row] >> 24;
        buffer = reverseBuffer(buffer);
        lc->setRow(3, row, buffer);
    }

    for (int row = 0; row < 8; row++){
        uint8_t buffer = data[row] >> 16;
        buffer = reverseBuffer(buffer);
        lc->setRow(2, row, buffer);
    }

    for (int row = 0; row < 8; row++){
        uint8_t buffer = data[row] >> 8;
        buffer = reverseBuffer(buffer);
        lc->setRow(1, row, buffer);
    }

    for (int row = 0; row < 8; row++){
        uint8_t buffer = data[row] >> 0;
        buffer = reverseBuffer(buffer);
        lc->setRow(0, row, buffer);
    }

    for (int row = 0; row < 8; row++){
        const uint8_t buffer = data[row+8] >> 24;
        lc->setRow(4, 7-row, buffer);
    }

    for (int row = 0; row < 8; row++){
        const uint8_t buffer = data[row+8] >> 16;
        lc->setRow(5, 7-row, buffer);
    }

    for (int row = 0; row < 8; row++){
        const uint8_t buffer = data[row+8] >> 8;
        lc->setRow(6, 7-row, buffer);
    }

    for (int row = 0; row < 8; row++){
        const uint8_t buffer = data[row+8] >> 0;
        lc->setRow(7, 7-row, buffer);
    }
}

void setPixel(uint32_t board[16], const Coordinate* coordinate, bool on){
    if (coordinate->x < 0 || coordinate->x > maxX || 
        coordinate->y < 0 || coordinate->y > maxY){
        return;
    }

    const uint32_t row = 1UL << (coordinate->y);
    if (on){
        board[15-coordinate->x] |= row;
    }
    else{
        board[15-coordinate->x] &= ~row;
    }
}

bool getPixel(uint32_t board[16], const Coordinate* coordinate){
    if (coordinate->x < 0 || coordinate->x > maxX || 
        coordinate->y < 0 || coordinate->y > maxY){
        return false;
    }

    if ((board[15-coordinate->x] & (1UL << coordinate->y)) != 0){
        return true;
    }

    return false;
}

bool checkRowWin(uint32_t board[16], const uint8_t row){
    if (row < 0 || row > 31){
        return false;
    }

    uint32_t win = 1UL << row;

    for (uint16_t i = 0; i < 16; ++i){
        win &= (board[i] << row);
    }

    return win;
}

void setRow(LedControl* lc, uint32_t board[16], const uint8_t rowNum, const bool on){
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
    flushBuffer(lc, board);
}

bool rowContains(uint32_t board[16], const uint8_t rowNum, const bool on){
    for (uint8_t i = 0; i < 16; ++i){
        Coordinate coord = {i, rowNum};
        const bool value = getPixel(board, &coord);
        if (value == on){
            return true;
        }
    }

    return false;
}

void moveRow(uint32_t board[16], const uint8_t rowNum, const uint8_t nextRowNum){
    for (uint8_t i = 0; i < 16; ++i){
        Coordinate coord = {i, rowNum};
        const bool value = getPixel(board, &coord);
        setPixel(board, &coord, false);
        coord.y = nextRowNum;
        setPixel(board, &coord, value);
    }
}

void clearBoard(LedControl* lc, uint32_t board[16]) {
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

    flushBuffer(lc, board);
}
#include "Display.hpp"

static constexpr uint8_t maxX = 15;
static constexpr uint8_t maxY = 31;

bool compareCoordinates(const Coordinate* c1, const Coordinate* c2){
    if (!c1 || !c2) {
        return false;
    }
    return c1->x == c2->x && c1->y == c2->y;
}

void flushBuffer(uint32_t data[16], LedControl* lc){
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
        uint8_t buffer = data[row+8] >> 24;
        lc->setRow(4, 7-row, buffer);
    }

    for (int row = 0; row < 8; row++){
        uint8_t buffer = data[row+8] >> 16;
        lc->setRow(5, 7-row, buffer);
    }

    for (int row = 0; row < 8; row++){
        uint8_t buffer = data[row+8] >> 8;
        lc->setRow(6, 7-row, buffer);
    }

    for (int row = 0; row < 8; row++){
        uint8_t buffer = data[row+8] >> 0;
        lc->setRow(7, 7-row, buffer);
    }
}

uint8_t reverseBuffer(uint8_t b){
    uint8_t rev = 0;
    for (uint8_t i = 0; i < 8; i++) {
        rev = (rev << 1) | (b & 1u);
        b >>= 1;
    }
    return rev;
}

void setPixel(uint32_t board[16], const Coordinate* coordinate, bool on){
    if (coordinate->x < 0 || coordinate->x > maxX || 
        coordinate->y < 0 || coordinate->y > maxY){
        return;
    }

    uint32_t row = 1UL << (coordinate->y);
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
    else{
        return false;
    }
}
#pragma once

#include <stdint.h>

struct Direction{
    int8_t dx;
    int8_t dy;
};

struct Coordinate{
    uint8_t x;
    uint8_t y;
};

bool compareCoordinates(const Coordinate* c1, const Coordinate*);
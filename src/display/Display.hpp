#pragma once

#include <Arduino.h>
#include <LedControl.h>

typedef struct{
    uint8_t x;
    uint8_t y;
} Coordinate;

bool compareCoordinates(const Coordinate*, const Coordinate*);
void flushBuffer(uint32_t[16], LedControl*);
uint8_t reverseBuffer(uint8_t);
void setPixel(uint32_t[16], const Coordinate*, bool);
bool getPixel(uint32_t[16], const Coordinate*);
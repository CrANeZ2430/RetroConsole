#include "Physics.hpp"

bool compareCoordinates(const Coordinate* c1, const Coordinate* c2){
    if (!c1 || !c2) {
        return false;
    }
    return c1->x == c2->x && c1->y == c2->y;
}
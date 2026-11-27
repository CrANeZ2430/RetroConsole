#include "Physics.hpp"

bool compareCoordinates(const Coordinate* coord1, const Coordinate* coord2){
    if (!coord1 || !coord2) {
        return false;
    }
    return coord1->x == coord2->x &&
        coord1->y == coord2->y;
}
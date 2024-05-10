#include "definitions/definitions.h"

#ifndef STRUCTS_H
#define STRUCTS_H

struct Instruction {
    double deltaThrottle;
    double deltaRoll;
    double deltaPitch;
    bool manualModeWingFlags[WING_COUNT];
};

#endif

#include <ESP32Servo.h>

#include "structs/structs.h"
#include "utils/defs.h"
#include "sensors/motionsensor.h"

#ifndef HAWK_H
#define HAWK_H

class Hawk {
    private:
        Servo wings[WING_COUNT];
        MotionSensor motionSensor;
        int wingPins[WING_COUNT];
        double wingThrottles[WING_COUNT];
        bool isCalibrating;

        void adjustThrottles();
        void balance();
        void printThrottles();

    public:
        Hawk(int wingPins[WING_COUNT]);
        
        void initialize();
        void calibrate();
        bool isFlying();
        void disableWings();
        void enableWings();
        void followInstruction(Instruction Instruction);
};

#endif
#include <MPU6050.h>

#include "structs/structs.h"

#ifndef MOTION_SENSE_H
#define MOTION_SENSE_H

class MotionSensor {
    private:
        MPU6050 sensor;
        double pitch;
        double roll;
        long startTime = millis();
        double previousAccelerationX;
        double previousAccelerationY;
        double previousAccelerationZ;

    public:
        MotionSensor();
        void initialize();
        void calibrate();
        void sense();
        double getRoll();
        double getPitch();
};

#endif
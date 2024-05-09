#include <MPU6050.h>

#include "structs/structs.h"

#ifndef MOTION_SENSE_H
#define MOTION_SENSE_H

class MotionSensor {
    private:
        MPU6050 sensor;
        long startTime;
        double previousGyroscopeX;
        double previousGyroscopeY;
        double previousGyroscopeZ;
        double previousAccelerationX;
        double previousAccelerationY;
        double previousAccelerationZ;
        double pitchFromGyroscope;
        double rollFromGyroscope;
        double pitchFromAccelerometer;
        double rollFromAccelerometer;

    public:
        MotionSensor();
        void initialize();
        void calibrate();
        void sense();
        double getRoll();
        double getPitch();
};

#endif
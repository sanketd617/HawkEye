#include <Adafruit_MPU6050.h>

#include "structs/structs.h"

#ifndef MOTION_SENSE_H
#define MOTION_SENSE_H

#define CALIBRATION_ALPHA 0.5
#define CALIBRATION_INTERVAL 10
#define CALIBRATION_SAMPLE_COUNT 100

class MotionSensor {
    private:
        Adafruit_MPU6050 sensor;
        float accelerationOffsetX;
        float accelerationOffsetY;
        float accelerationOffsetZ;
        float gyroOffsetX;
        float gyroOffsetY;
        float gyroOffsetZ;
        float rollOffset;
        float pitchOffset;
        float pitch = 0;
        float roll = 0;
        long startTime = millis();

    public:
        MotionSensor();
        void calibrate();
        void sense();
        float getRoll();
        float getPitch();
};

#endif
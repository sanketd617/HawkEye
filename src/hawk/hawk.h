#include <ESP32Servo.h>
#include "utils/defs.h"

#ifndef HAWK_H
#define HAWK_H

#define MOTOR_COUNT 4
#define MAX_MOTOR_SPEED 100.0
#define MIN_MOTOR_SPEED 0.0

class Hawk {
    private:
        Servo motors[MOTOR_COUNT];
        int pins[MOTOR_COUNT];
        float motorSpeeds[4];

        void spinAllMotors(float speed);

    public:
        Hawk(int pins[MOTOR_COUNT]);
        void calibrate();
        bool isFlying();
};

#endif
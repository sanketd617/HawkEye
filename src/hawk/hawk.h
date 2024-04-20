#include <ESP32Servo.h>
#include "utils/defs.h"

#ifndef HAWK_H
#define HAWK_H

#define MOTOR_COUNT 4

class Hawk {
    private:
        Servo motors[MOTOR_COUNT];
        int pins[MOTOR_COUNT];

        void spinAllMotors(float speed);

    public:
        Hawk(int pins[MOTOR_COUNT]);
        void calibrate();
};

#endif
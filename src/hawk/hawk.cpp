#include <math.h>

#include "hawk/hawk.h"
#include "utils/defs.h"
#include "utils/utils.h"

Hawk::Hawk(int pins[MOTOR_COUNT]) {
    for (int i = 0; i < MOTOR_COUNT; i++) {
        this->pins[i] = pins[i];
    }
    
    for (int i = 0; i < MOTOR_COUNT; i++) {
        motors[i].attach(pins[i], MIN_PULSE, MAX_PULSE);
    }
}

void Hawk::calibrate() {
    Serial.println("Calibrating motors..");
    blink(1000, 10000);
    spinAllMotors(100.0);
    blink(10, 10000);
    spinAllMotors(0.0);
    blink(1000, 5000);
    spinAllMotors(10.0);
    blink(10, 5000);
    spinAllMotors(0.0);
    Serial.println("Motors calibrated..");
}

void Hawk::spinAllMotors(float speed) {
    if (speed > 100.0 || speed < 0.0) {
        throw std::runtime_error("Invalid speed! Acceptable range is [0.0 to 100.0]. Given speed: " + std::to_string(speed));
    }
    int pulse = MIN_PULSE + speed * 10;
    for (int i = 0; i < MOTOR_COUNT; i++) {
        motors[i].writeMicroseconds(pulse);
    }
}

#include <math.h>

#include "hawk/hawk.h"
#include "utils/utils.h"
#include "utils/defs.h"
#include "structs/structs.h"

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
    blink(100, 1000); // TODO: Restart the motors
    spinAllMotors(100.0);
    blink(100, 10000);
    spinAllMotors(0.0);
    blink(100, 5000);
    spinAllMotors(10.0);
    blink(100, 5000);
    spinAllMotors(0.0);
    Serial.println("Motors calibrated..");
}

void Hawk::spinAllMotors(float speed) {
    if (speed > MAX_MOTOR_SPEED || speed < MIN_MOTOR_SPEED) {
        throw std::runtime_error("Invalid speed! Acceptable range is [0.0 to 100.0]. Given speed: " + std::to_string(speed));
    }
    int pulse = MIN_PULSE + speed * 10;
    for (int i = 0; i < MOTOR_COUNT; i++) {
        motorSpeeds[i] = speed;
        motors[i].writeMicroseconds(pulse);
    }
}

bool Hawk::isFlying() {
    for (int i = 0; i < MOTOR_COUNT; i++) {
        if (motorSpeeds[i] != 0) {
            return true;
        }
    }
    return false;
}

void Hawk::move(Speed speed) {
    spinAllMotors(speed.y);
}
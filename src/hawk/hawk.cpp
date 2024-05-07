#include <math.h>

#include "hawk/hawk.h"
#include "utils/utils.h"
#include "utils/defs.h"
#include "structs/structs.h"

Hawk::Hawk(int pins[WING_COUNT]) {
    for (int i = 0; i < WING_COUNT; i++) {
        wingPins[i] = pins[i];
    }
    
    for (int i = 0; i < WING_COUNT; i++) {
        wings[i].attach(pins[i], MIN_PULSE, MAX_PULSE);
    }
    isCalibrating = false;
}

void Hawk::initialize() {
    pinMode(WING_SWITCH_PIN, OUTPUT);
    motionSensor.initialize();
}

void Hawk::disableWings() {
    digitalWrite(WING_SWITCH_PIN, HIGH);
}

void Hawk::enableWings() {
    digitalWrite(WING_SWITCH_PIN, LOW);
}

void Hawk::calibrate() {
    disableWings();

    isCalibrating = true;
    Serial.println("Calibrating motion sensor..");
    motionSensor.calibrate();
    Serial.println("Calibrating motors..");

    std::fill(wingThrottles, wingThrottles + WING_COUNT, 100.0);
    adjustThrottles();
    enableWings();
    blink(1000, 2000);
    std::fill(wingThrottles, wingThrottles + WING_COUNT, 0.0);
    adjustThrottles();
    blink(1000, 2000);
    std::fill(wingThrottles, wingThrottles + WING_COUNT, 5.0);
    adjustThrottles();
    blink(1000, 5000);
    std::fill(wingThrottles, wingThrottles + WING_COUNT, 0.0);
    adjustThrottles();
    Serial.println("Calibration finish");
    isCalibrating = false;
}

void Hawk::adjustThrottles() {
    if (!isCalibrating) {
        balance();
    }
    
    printThrottles();
    for (int i = 0; i < WING_COUNT; i++) {
        if (wingThrottles[i] > MAX_WING_THROTTLE || wingThrottles[i] < MIN_WING_THROTTLE) {
            Serial.println("Invalid throttle! Acceptable range is [0.0 to 100.0]");
            throw std::runtime_error("Invalid throttle! Acceptable range is [0.0 to 100.0]");
        }
    }
    
    for (int i = 0; i < WING_COUNT; i++) {
        int pulse = MIN_PULSE + wingThrottles[i] * 10;
        wingThrottles[i] = wingThrottles[i];
        wings[i].writeMicroseconds(pulse);
    }
}

bool Hawk::isFlying() {
    for (int i = 0; i < WING_COUNT; i++) {
        if (wingThrottles[i] != 0) {
            return true;
        }
    }
    return false;
}

void Hawk::followInstruction(Instruction instruction) {
    if (instruction.activeWing == ALL_WINGS) {
        for (int i = 0; i < WING_COUNT; i++) {
            wingThrottles[i] = min(max(MIN_WING_THROTTLE, (double) wingThrottles[i] + instruction.deltaSpeed), LIMITED_MAX_WING_THROTTLE);
        }
        adjustThrottles();
        return;
    }
    wingThrottles[instruction.activeWing] = min(max(MIN_WING_THROTTLE, (double) wingThrottles[instruction.activeWing] + instruction.deltaSpeed), LIMITED_MAX_WING_THROTTLE);
    adjustThrottles();
}

void Hawk::balance() {
    motionSensor.sense();
    Serial.printf("Roll: %f\tPitch: %f\n", motionSensor.getRoll(), motionSensor.getPitch());
}

void Hawk::printThrottles() {
    Serial.printf("\t%.2f\t%.2f\n", wingThrottles[FRONT_LEFT_WING], wingThrottles[FRONT_RIGHT_WING]);
    Serial.println("            \\  /");
    Serial.println("            /  \\");
    Serial.printf("\t%.2f\t%.2f\n", wingThrottles[REAR_LEFT_WING], wingThrottles[REAR_RIGHT_WING]);
}

#include <ps5Controller.h>
#include <esp32-hal-timer.h>
#include <math.h>

#include "dualsense.h"
#include "utils/utils.h"
#include "utils/defs.h"

DualSense::DualSense() {
    calibrationRequestStartTime = DEFAULT_CALIBRATION_START_TIME;
    calibrationInProgress = false;
    rampUpStartTime = DEFAULT_RAMP_UP_START_TIME;
}

void DualSense::connect() {
    int attempts = 0;

    ps5.begin(DUALSENSE_MAC_ADDRESS);

    while (!ps5.isConnected() && attempts < DUALSENSE_MAX_CONNECTION_ATTEMPTS) {
        attempts++;
        Serial.printf("[Attempt %d / %d] Connecting to DualSense controller..\n", attempts, DUALSENSE_MAX_CONNECTION_ATTEMPTS);
        blink(500, 500);
    }

    if (attempts >= DUALSENSE_MAX_CONNECTION_ATTEMPTS) {
        Serial.println("Failed to connect to DualSense controller!");
    } else {
        Serial.println("Connected to DualSense controller!");
    }
    
}

bool DualSense::isConnected() {
    return ps5.isConnected();
}

bool DualSense::isCalibrationRequested() {
    if (calibrationInProgress) {
        return false;
    }
    if (ps5.L1() && ps5.L2() && ps5.R1() && ps5.R2()) {
        if (calibrationRequestStartTime == DEFAULT_CALIBRATION_START_TIME) {
            calibrationRequestStartTime = esp_timer_get_time();
        }
        double elapsedTime = (double) (esp_timer_get_time () - calibrationRequestStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime >= CALIBRATION_DURATION_IN_SECONDS) {
            calibrationInProgress = true;
            return true;
        }
    } else {
        calibrationRequestStartTime = DEFAULT_CALIBRATION_START_TIME;
    }
    return false;
}

void DualSense::resetCalibrationFlags() {
    calibrationRequestStartTime = DEFAULT_CALIBRATION_START_TIME;
    calibrationInProgress = false;
}

Instruction DualSense::readInstruction() {
    long currentTime = millis();
    long elapsedTime = currentTime - rampUpStartTime;

    double deltaThrottle = 0;

    if (ps5.R2() && rampUpStartTime != DEFAULT_RAMP_UP_START_TIME) {
        deltaThrottle += ((double) ps5.R2Value() / MAX_L2_AND_R2_VALUE) * elapsedTime * RAMP_UP_STEP_SIZE;
    }

    if (ps5.L2() && rampUpStartTime != DEFAULT_RAMP_UP_START_TIME) {
        deltaThrottle -= ((double) ps5.L2Value() / MAX_L2_AND_R2_VALUE) * elapsedTime * 2 * RAMP_UP_STEP_SIZE;
    }

    double deltaRoll = 0; 
    if (abs(ps5.LStickX()) > 20) {
        deltaRoll = (double) ps5.LStickX() / (ps5.LStickX() < 0 ? -1 * MIN_JOYSTICK_VALUE : MAX_JOYSTICK_VALUE) * elapsedTime * ROLL_AND_PITCH_STEP_SIZE;
    }

    double deltaPitch = 0;
    if (abs(ps5.LStickY()) > 20) {
        deltaPitch = (double) ps5.LStickY() / (ps5.LStickY() < 0 ? -1 * MIN_JOYSTICK_VALUE : MAX_JOYSTICK_VALUE) * elapsedTime * ROLL_AND_PITCH_STEP_SIZE;
    }

    int activeWing = ALL_WINGS;

    if (ps5.Cross()) {
        activeWing = REAR_RIGHT_WING;
    }

    if (ps5.Circle()) {
        activeWing = FRONT_RIGHT_WING;
    }

    if (ps5.Triangle()) {
        activeWing = FRONT_LEFT_WING;
    }

    if (ps5.Square()) {
        activeWing = REAR_LEFT_WING;
    }

    rampUpStartTime = currentTime;
    return {deltaThrottle, deltaRoll, deltaPitch, activeWing};
}

#include <ps5Controller.h>
#include <esp32-hal-timer.h>
#include <math.h>

#include "dualsense.h"
#include "utils/utils.h"

DualSense::DualSense() {
    callibrationRequestStartTime = CALIBRATION_NOT_REQUESTED_FLAG;
    calibrationInProgress = false;
    rampUpStartTime = RAMP_UP_NOT_REQUESTED_FLAG;
    speed = {0.0, 0.0, 0.0};
}

void DualSense::connect() {
    int attempts = 0;

    ps5.begin(DUALSENSE_MAC_ADDRESS);

    while (!ps5.isConnected() && attempts < DUALSENSE_MAX_CONNECTION_ATTEMPTS) {
        attempts++;
        Serial.printf("[Attempt %d / %d] Trying to connect to DualSense controller..\n", attempts, DUALSENSE_MAX_CONNECTION_ATTEMPTS);
        blink(1000, 1000);
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
        if (callibrationRequestStartTime == CALIBRATION_NOT_REQUESTED_FLAG) {
            callibrationRequestStartTime = esp_timer_get_time();
        }
        double elapsedTime = (double) (esp_timer_get_time () - callibrationRequestStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime >= CALIBRATION_THRESHOLD) {
            calibrationInProgress = true;
            return true;
        }
    } else {
        callibrationRequestStartTime = CALIBRATION_NOT_REQUESTED_FLAG;
    }
    return false;
}

void DualSense::resetCalibrationFlags() {
    callibrationRequestStartTime = CALIBRATION_NOT_REQUESTED_FLAG;
    calibrationInProgress = false;
}

Speed DualSense::getRequestedSpeed() {
    long currentTime = millis();
    long elapsedTime = currentTime - rampUpStartTime;

    if (ps5.R2() && rampUpStartTime != RAMP_UP_NOT_REQUESTED_FLAG) {
        double deltaY = ((double) ps5.R2Value() / MAX_L2_R2_VALUE) * elapsedTime * RAMP_UP_STEP_SIZE;
        speed.y = min(speed.y + deltaY, MAX_MOTOR_SPEED);
    }

    if (ps5.L2() && rampUpStartTime != RAMP_UP_NOT_REQUESTED_FLAG) {
        double deltaY = ((double) ps5.L2Value() / MAX_L2_R2_VALUE) * elapsedTime * 2 * RAMP_UP_STEP_SIZE;
        speed.y = max(speed.y - deltaY, MIN_MOTOR_SPEED);
    }

    rampUpStartTime = currentTime;
    return speed;
}

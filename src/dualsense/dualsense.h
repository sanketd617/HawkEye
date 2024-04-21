#include "structs/structs.h"

#ifndef DUALSENSE_H
#define DUALSENSE_H

#define DUALSENSE_MAC_ADDRESS "4C:B9:9B:FB:F5:9A"
#define DUALSENSE_MAX_CONNECTION_ATTEMPTS 60
#define CALIBRATION_NOT_REQUESTED_FLAG -1
#define CALIBRATION_THRESHOLD 5.0
#define MAX_MOTOR_SPEED 30.0
#define MIN_MOTOR_SPEED 0.0
#define MAX_L2_R2_VALUE 255
#define RAMP_UP_DURATION 5000.0
#define RAMP_UP_NOT_REQUESTED_FLAG -1
#define RAMP_UP_STEP_SIZE (double) MAX_MOTOR_SPEED / RAMP_UP_DURATION;

class DualSense {
    private:
        int callibrationRequestStartTime;
        bool calibrationInProgress;
        long rampUpStartTime;
        Speed speed;
    public:
        DualSense();
        void connect();
        bool isConnected();
        bool isCalibrationRequested();
        void resetCalibrationFlags();
        Speed getRequestedSpeed();
};

#endif
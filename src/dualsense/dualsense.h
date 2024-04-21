#ifndef DUALSENSE_H
#define DUALSENSE_H

#define DUALSENSE_MAC_ADDRESS "4C:B9:9B:FB:F5:9A"
#define DUALSENSE_MAX_CONNECTION_ATTEMPTS 60
#define CALIBRATION_NOT_REQUESTED_FLAG -1
#define CALIBRATION_THRESHOLD 5.0

class DualSense {
    private:
        int callibrationRequestStartTime;
        bool calibrationInProgress;
    public:
        DualSense();
        void connect();
        bool isConnected();
        bool isCalibrationRequested();
        void resetCalibrationFlags();
};

#endif
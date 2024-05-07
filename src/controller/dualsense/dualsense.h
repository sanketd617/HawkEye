#include "structs/structs.h"
#include "definitions/definitions.h"

#ifndef DUALSENSE_H
#define DUALSENSE_H

class DualSense {
    private:
        long calibrationRequestStartTime;
        bool calibrationInProgress;
        long rampUpStartTime;
    public:
        DualSense();

        void connect();
        bool isConnected();
        bool isCalibrationRequested();
        void resetCalibrationFlags();
        Instruction readInstruction();
};

#endif
#include "structs/structs.h"
#include "utils/defs.h"

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
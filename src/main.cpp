#include <Arduino.h>

#include "utils/defs.h"
#include "hawk/hawk.h"
#include "dualsense/dualsense.h"
#include "sensors/motionsensor.h"
#include "structs/structs.h"

Hawk hawk((int[]) HAWK_PINS);
DualSense dualSense;

MotionSensor motionSensor;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  motionSensor.calibrate();

  dualSense.connect();
}

void loop() {
  if (!dualSense.isConnected()) {
    // Do nothing if DualSense controller is not connected
    return;
  }

  if (dualSense.isCalibrationRequested() && !hawk.isFlying()) {
    hawk.calibrate();
    dualSense.resetCalibrationFlags();
    return;
  }

  motionSensor.sense();
  hawk.move(dualSense.getRequestedSpeed(), motionSensor.getRoll(), motionSensor.getPitch());
}

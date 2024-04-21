#include <Arduino.h>

#include "utils/defs.h"
#include "hawk/hawk.h"
#include "dualsense/dualsense.h"
#include "structs/structs.h";

Hawk hawk((int[]) HAWK_PINS);
DualSense dualSense;

void setup() {
  Serial.begin(115200);
  while(!Serial);

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

  Speed speed = dualSense.getRequestedSpeed();
  hawk.move(speed);
}

#include <Arduino.h>

#include "definitions/definitions.h"
#include "hawk/hawk.h"
#include "controller/dualsense/dualsense.h"
#include "hawk/sensors/motion/motion.h"
#include "structs/structs.h"

Hawk hawk((int[]) HAWK_PINS);
DualSense dualSense;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  delay(100);
  Wire.begin();
  delay(100);
  dualSense.connect();
  hawk.initialize();
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

  hawk.followInstruction(dualSense.readInstruction());
}

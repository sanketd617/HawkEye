#include <Arduino.h>
#include "utils/defs.h"
#include "hawk/hawk.h"

Hawk hawk((int[]) HAWK_PINS);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  hawk.calibrate();
}

void loop() {
  
}

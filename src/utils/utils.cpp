#include <Arduino.h>
#include "defs.h"

void blink(int blinkRate, int blinkDuration) {
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < blinkDuration; i += blinkRate) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay((float) blinkRate / 2);
    digitalWrite(LED_BUILTIN, LOW);
    delay((float) blinkRate / 2);
  }
}
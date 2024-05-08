#include <Arduino.h>
#include "definitions/definitions.h"

void blink(int blinkRate, int blinkDuration) {
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < blinkDuration; i += blinkRate) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay((double) blinkRate / 2);
    digitalWrite(LED_BUILTIN, LOW);
    delay((double) blinkRate / 2);
  }
}

double lowPassFilter(double currentValue, double previousValue, double alpha) {
  return alpha * currentValue + (1 - alpha) * (previousValue == UNDEFINED ? currentValue : previousValue);
}
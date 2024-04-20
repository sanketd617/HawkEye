#include <Arduino.h>
#include "defs.h"

void blink(int blinkRate, int blinkDuration) {
  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < blinkDuration; i += blinkRate) {
    digitalWrite(LED_PIN, HIGH);
    delay((float) blinkRate / 2);
    digitalWrite(LED_PIN, LOW);
    delay((float) blinkRate / 2);
  }
}
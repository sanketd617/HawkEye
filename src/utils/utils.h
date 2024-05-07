#ifndef UTILS_H

#define UTILS_H

void blink(int blinkRate, int blinkDuration);
double lowPassFilter(double currentValue, double previousValue, double alpha);

#endif

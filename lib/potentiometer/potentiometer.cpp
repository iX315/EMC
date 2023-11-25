#include "potentiometer.hpp"

Potentiometer::Potentiometer() {
}

int Potentiometer::readValue() {
  return analogRead(POTENTIOMETER_PIN);
}

int Potentiometer::readMappedValue(int min, int max) {
  return map(analogRead(POTENTIOMETER_PIN), 0, 1023, min, max);
}
#include "potentiometer.hpp"

#ifdef ESP8266
#define MAX_RESOLUTION 1023
#endif

#ifdef ESP32
#define MAX_RESOLUTION 4095
#endif

Potentiometer::Potentiometer() {
}

int Potentiometer::readValue() {
  return analogRead(POTENTIOMETER_PIN);
}

int Potentiometer::readMappedValue(int min, int max) {
  return map(analogRead(POTENTIOMETER_PIN), 0, MAX_RESOLUTION, min, max);
}
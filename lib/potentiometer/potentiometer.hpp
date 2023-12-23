#include "../constants.hpp"

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#ifdef ESP8266
#define MAX_RESOLUTION 1023
#endif

#ifdef ESP32
#define MAX_RESOLUTION 4095
#endif

class Potentiometer {
private:
  int moveToValue = -1;
  int lastValue = 0;
  int minValue = 0;
  int maxValue = MAX_RESOLUTION;
public:
  Potentiometer();
  int readValue();
  int readMappedValue(int min, int max);
  bool shouldMove();
  void motorMove(int value);
  bool isTouched();
  void loop();
  bool hasChanged();
  void calibrate();
};

#endif // POTENTIOMETER_H

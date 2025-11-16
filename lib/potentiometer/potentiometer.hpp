#include "../constants.hpp"

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#ifdef ESP8266
#define MAX_RESOLUTION 1023
#endif

#ifdef ESP32
#define MAX_RESOLUTION 4095
#endif

class TouchSense {
public:
  TouchSense(int apin, int athreshold = 300) {
    pin = apin;
    thold = athreshold;
  }
  void begin() {
    baseline = readTouch();
  }
  int readTouch() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    pinMode(pin, INPUT);
    int i = 0;
    while (digitalRead(pin)) {
      i++;
    }
    return i;
  }
  bool isTouched() {
    return (readTouch() > baseline + thold);
  }
  int baseline;
  int thold;
  int pin;
};

class Potentiometer {
private:
  int lastPosition = 0;
  int position = 0;
  float filterAmt = 0.75;
  float speed = 1.0;
  bool motorReleaseState = false;

  int minCalValue = 0;
  int maxCalValue = MAX_RESOLUTION;

  TouchSense touchSense = TouchSense(POTENTIOMETER_TOUCH, 300);
  void goToPosition(int position);
  void disableMotor();
  void enableMotor(int direction);
public:
  Potentiometer();
  int readValue();
  bool isTouched();
  int loop(int newPosition);
  int midiValueToPotValue(int value);
  int potValueToMidiValue(int value);
  void calibrate();
};

#endif // POTENTIOMETER_H

#include "potentiometer.hpp"

Potentiometer::Potentiometer() {
  // initialize motor fader
  pinMode(L293D_1A, OUTPUT);
  pinMode(L293D_2A, OUTPUT);
}

bool Potentiometer::isTouched() {
  return analogRead(POTENTIOMETER_TOUCH);
}

int Potentiometer::readValue() {
  return analogRead(POTENTIOMETER_PIN);
}

int Potentiometer::readMappedValue(int min, int max) {
  return map(readValue(), 0, MAX_RESOLUTION, min, max);
}

bool Potentiometer::shouldMove() {
  if (isTouched()) {
    return false;
  }
  if (moveToValue == -1) {
    return false;
  }
  return abs(readValue() - moveToValue) > HYSTERESIS;
}

void Potentiometer::loop() {
  if (shouldMove()) {
    if (readValue() < moveToValue) {
      digitalWrite(L293D_1A, LOW);
      digitalWrite(L293D_2A, HIGH);
    }

    if (readValue() > moveToValue) {
      digitalWrite(L293D_1A, HIGH);
      digitalWrite(L293D_2A, LOW);
    }
  } else {
    digitalWrite(L293D_1A, LOW);
    digitalWrite(L293D_2A, LOW);
    moveToValue = -1;
  }

  lastValue = readValue();
}

void Potentiometer::motorMove(int value) {
  int newMoveValue = map(value, MIN_VALUE, MAX_VALUE, minValue, maxValue);
  moveToValue = shouldMove() ? newMoveValue : -1;
}

bool Potentiometer::hasChanged() {
  if (lastValue != readValue()) {
    lastValue = readValue();
    return true;
  }
  return false;
}

void Potentiometer::calibrate() {
  //Send fader to the top and read max position
  digitalWrite(L293D_1A, LOW);
  digitalWrite(L293D_2A, HIGH);
  delay(1000);
  maxValue = readValue();

  //Send fader to the bottom and read max position
  digitalWrite(L293D_1A, HIGH);
  digitalWrite(L293D_2A, LOW);
  delay(1000);
  
  minValue = readValue();

  digitalWrite(L293D_1A, LOW);
  digitalWrite(L293D_2A, LOW);
}
#include "potentiometer.hpp"

Potentiometer::Potentiometer() {
  // initialize motor fader
  pinMode(L293D_1A, OUTPUT);
  pinMode(L293D_2A, OUTPUT);
}

bool Potentiometer::isTouched() {
  return touchSense.isTouched();
}

int Potentiometer::readValue() {
  int reading = analogRead(POTENTIOMETER_PIN);
  return int( (filterAmt * lastPosition) + ( (1.0 - filterAmt) * int(reading / 4 )) );
}

int Potentiometer::midiValueToPotValue(int value) {
  return map(value, MIN_VALUE, MAX_VALUE, minCalValue, maxCalValue);
}

int Potentiometer::potValueToMidiValue(int value) {
  return map(value, minCalValue, maxCalValue, MIN_VALUE, MAX_VALUE);
}

void Potentiometer::disableMotor() {
  motorReleaseState = true;
  digitalWrite(L293D_1A, LOW);
  digitalWrite(L293D_2A, LOW);
}

void Potentiometer::enableMotor(int direction) {
  motorReleaseState = false;
  if (direction > 0) {
    digitalWrite(L293D_1A, HIGH);
    digitalWrite(L293D_2A, LOW);
  } else {
    digitalWrite(L293D_1A, LOW);
    digitalWrite(L293D_2A, HIGH);
  }
}

int Potentiometer::loop(int newPosition = 0) {
  if (isTouched()) {
    disableMotor();
    return readValue();
  }

#ifdef DEBUG
  Serial.println("pot loop: " + String(newPosition));
#endif

  if (abs(position - newPosition) > HYSTERESIS) {
    if (motorReleaseState == false) {
      goToPosition(newPosition);
    }
  }

  return readValue();
}

void Potentiometer::goToPosition(int newPosition = 0) {
#ifdef DEBUG
  Serial.println("goToPosition: " + String(newPosition) + " from " + String(readValue()));
#endif

  if (abs(readValue() - newPosition) > 4) {
    if (readValue() > newPosition) {
      speed = 2.25 * abs(readValue() - newPosition) / MAX_VALUE + 0.2;
      speed = constrain(speed, -1.0, 1.0);
      if (speed > 0.0) {
        enableMotor(-1);
      }
    }
    if (readValue() < newPosition) {
      speed = 2.25 * abs(readValue() - newPosition) / MAX_VALUE - 0.2;
      speed = constrain(speed, -1.0, 1.0);
      if (speed > 0.0) {
        enableMotor(1);
      }
    }
  } else {
    disableMotor();
  }
}

void Potentiometer::calibrate() {
  //Send fader to the bottom and read max position
  enableMotor(-1);
  delay(1000);
  // read raw value from potentiometer
  minCalValue = analogRead(POTENTIOMETER_PIN);

  //Send fader to the top and read max position
  enableMotor(1);
  delay(1000);
  // read raw value from potentiometer
  maxCalValue = analogRead(POTENTIOMETER_PIN);

  // go back to the middle - test
  goToPosition(maxCalValue / 2);

#ifdef DEBUG
  Serial.println("Calibrated potentiometer");
  Serial.println("minCalValue: " + String(minCalValue));
  Serial.println("maxCalValue: " + String(maxCalValue));
#endif
}
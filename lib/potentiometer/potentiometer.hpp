#include "../constants.hpp"

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

class Potentiometer {
private:
public:
  Potentiometer();
  int readValue();
  int readMappedValue(int min, int max);
};

#endif // POTENTIOMETER_H

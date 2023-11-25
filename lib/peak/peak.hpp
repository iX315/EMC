#include "../constants.hpp"
#include <FastLED.h>

#ifndef PEAK_H
#define PEAK_H

class Peak {
private:
  CRGB leds[NUM_LEDS];

public:
  Peak();
  void loop(int velocity);
};

#endif // PEAK_H

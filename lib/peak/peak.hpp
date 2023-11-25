#include <FastLED.h>
#include "../constants.hpp"

#ifndef PEAK_H
#define PEAK_H

class Peak {
private:
  CRGB leds[NUM_LEDS];
public:
  Peak();
};

#endif // PEAK_H

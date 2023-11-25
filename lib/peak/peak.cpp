#include "peak.hpp"

Peak::Peak() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
}
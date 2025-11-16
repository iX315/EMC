#include "peak.hpp"

Peak::Peak() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(10);
  fill_solid(&(leds[0]), NUM_LEDS, CRGB::Black);
}

void Peak::loop(int value) {
  // Update LED peak based on MIDI note velocity
  int peakLevel = map(value, 0, 127, 0, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < peakLevel) {
        leds[i] = CRGB::Green;
        if (i > 6) {
          leds[i] = CRGB::Yellow;
        }
        if (i > 8) {
          leds[i] = CRGB::Red;
        }
    } else {
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
}

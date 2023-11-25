#include "peak.hpp"

Peak::Peak() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);\
    FastLED.setBrightness(30);
    fill_solid(&(leds[0]), NUM_LEDS, CRGB::Black);
}

void Peak::loop(int velocity) {
    // Update LED peak based on MIDI note velocity
    int peakLevel = map(velocity, 0, 127, 0, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i < peakLevel) {
            leds[i] = CRGB::Green;
        } else {
            leds[i] = CRGB::Black;
        }
    }
    FastLED.show();
}

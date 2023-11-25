#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include "AiEsp32RotaryEncoder.h"
#include "../constants.h"
#include "../utils.h"

#ifndef ROTARY_H
#define ROTARY_H

class Rotary {
private:
    AiEsp32RotaryEncoder _rotaryEncoder;
    static AiEsp32RotaryEncoder* rotaryEncoder;
    int _newPosition;
    int _oldPosition;
public:
    Rotary();

    static void IRAM_ATTR readEncoderISR() {
        rotaryEncoder->readEncoder_ISR();
    };

    void init();
    int getEncoderValue();
    void loop(void (*callback)());
};

#endif // ROTARY_H
#include <TFT_eSPI.h>
#include "AiEsp32RotaryEncoder.h"

#ifndef ROTARY_H
#define ROTARY_H

typedef void (*actionCallback)(void);
static void noop(void) { };

#define MIN_CONTROL 0
#define MAX_CONTROL 127
#define MIN_VALUE 0
#define MAX_VALUE 127
#define MIN_CHANNEL 1
#define MAX_CHANNEL 16

#define ROTARY_ENCODER_A_PIN          PIN_D3 // CLK (A pin)
#define ROTARY_ENCODER_B_PIN          PIN_D0 // DT (B pin)
#define ROTARY_ENCODER_BUTTON_PIN     -1
#define ROTARY_ENCODER_VCC_PIN        -1
#define ROTARY_ENCODER_STEPS          1
#define ROTARY_ENCODER_ACCELERATION   10

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

    actionCallback callback = noop;

    void init();
    int getEncoderValue();
    void loop(actionCallback callback = noop);
};

#endif // ROTARY_H
#include "rotary.h"

Rotary::Rotary() {
    _rotaryEncoder = AiEsp32RotaryEncoder(
        ROTARY_ENCODER_A_PIN,
        ROTARY_ENCODER_B_PIN,
        ROTARY_ENCODER_BUTTON_PIN,
        ROTARY_ENCODER_VCC_PIN,
        ROTARY_ENCODER_STEPS
    );
    rotaryEncoder = &_rotaryEncoder;
    _newPosition = 15;
    _oldPosition = 0;
}

AiEsp32RotaryEncoder* Rotary::rotaryEncoder = nullptr;

void Rotary::init() {
    // initialize rotary encoder
    _rotaryEncoder.begin();

    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), readEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_B_PIN), readEncoderISR, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_BUTTON_PIN), rotary_onButtonClick, RISING);

    // max, min, circleValues true|false (when max go to min and vice versa)
    _rotaryEncoder.setBoundaries(MIN_VALUE, MAX_VALUE, true); 
    // set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
    _rotaryEncoder.setAcceleration(ROTARY_ENCODER_ACCELERATION);
}

int Rotary::getEncoderValue() {
    return _newPosition;
}

void Rotary::loop(void (*callback)()) {
    // dont do anything unless value changed
    if (!_rotaryEncoder.encoderChanged()) {
        return;
    } else {
        _newPosition = _rotaryEncoder.readEncoder();

        callback();
    }
}
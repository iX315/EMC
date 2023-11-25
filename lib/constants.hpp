#include "Arduino.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MIN_CONTROL 0
#define MAX_CONTROL 127
#define MIN_VALUE 0
#define MAX_VALUE 127
#define MIN_CHANNEL 1
#define MAX_CHANNEL 16

#define ROTARY_ENCODER_A_PIN PIN_D3 // CLK (A pin)
#define ROTARY_ENCODER_B_PIN PIN_D0 // DT (B pin)
#define ROTARY_ENCODER_BUTTON_PIN -1
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 1
#define ROTARY_ENCODER_ACCELERATION 1

#define NUM_LEDS 11
#define LED_PIN 2 // D4

#define POTENTIOMETER_PIN PIN_A0

//#define USE_UI
//#define USE_ROTARY
#define USE_POTENTIOMETER
#define USE_PEAK
//#define USE_TOUCH

#endif // CONSTANTS_H
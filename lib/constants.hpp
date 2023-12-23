#include "Arduino.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MIN_VALUE 0
#define MAX_VALUE 16256
#define MIN_CONTROL 1
#define MAX_CONTROL 127
#define MIN_CHANNEL 1
#define MAX_CHANNEL 16
#define HYSTERESIS 10

#ifdef ESP8266
#define ROTARY_ENCODER_A_PIN D3 // CLK (A pin)
#define ROTARY_ENCODER_B_PIN D0 // DT (B pin)
#define POTENTIOMETER_PIN A0
#define POTENTIOMETER_TOUCH D0
#define LED_PIN D4
#endif

#ifdef ESP32
#define ROTARY_ENCODER_A_PIN 15 // CLK (A pin)
#define ROTARY_ENCODER_B_PIN 0 // DT (B pin)
#define POTENTIOMETER_PIN 15
#define POTENTIOMETER_PIN // TODO
#define LED_PIN 4
#endif

#define ROTARY_ENCODER_BUTTON_PIN -1
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 1
#define ROTARY_ENCODER_ACCELERATION 1

#ifdef ESP8266
#define L293D_ENABLE_1_2 -1
#define L293D_1A D1
#define L293D_2A D2
#endif

#define NUM_LEDS 11

//#define USE_UI
//#define USE_ROTARY
#define USE_POTENTIOMETER
#define USE_PEAK
//#define USE_TOUCH

//#define DEBUG_MIDI
#define DEBUG_INPUTS

#endif // CONSTANTS_H
#include <rotary.hpp>
#include <ui.hpp>
#include <touch.hpp>

#include <SoftwareSerial.h>
#include <MIDI.h>
#include <SPI.h>
#include <TFT_eSPI.h>

int currentControl = MIN_CONTROL;
int currentChannel = MIN_CHANNEL;

TFT_eSPI tft = TFT_eSPI();

Ui ui = Ui(&tft, &currentControl, &currentChannel);
Rotary rotary = Rotary();
Touch touch = Touch(&tft, &ui);

MIDI_CREATE_DEFAULT_INSTANCE();

void rotary_onValueChanged() {
  MIDI.sendControlChange(currentControl, rotary.getEncoderValue(), currentChannel);
  ui.toggleShouldUpdate();
}

static void btnL_pressAction(void) {
  if (ui.btnL.justReleased()) {
   ui.decreaseControl();
  }
}

static void btnR_pressAction(void) {
  if (ui.btnR.justReleased()) {
    ui.increaseControl();
  }
}

void setup() {
  Serial.println("Hello!");
  ui.init();
  MIDI.begin(1);
  Serial.begin(115200);

  rotary.init();

  ui.update();
  ui.initButtons(btnL_pressAction, btnR_pressAction);
  touch.calibrate();
}

void loop() {
  rotary.loop(rotary_onValueChanged);
  touch.loop();
  ui.update(rotary.getEncoderValue(), currentControl, currentChannel);
}

#include <MIDI.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TFT_eSPI.h>

#include <peak.hpp>
#include <potentiometer.hpp>
#include <rotary.hpp>
#include <touch.hpp>
#include <ui.hpp>

int currentValue = MIN_VALUE;
int currentControl = MIN_CONTROL;
int currentChannel = MIN_CHANNEL;

TFT_eSPI tft = TFT_eSPI();

#ifdef USE_UI
Ui ui = Ui(&tft, &currentControl, &currentChannel);
#endif

#ifdef USE_ROTARY
Rotary rotary = Rotary();
#endif

#ifdef USE_PEAK
Peak peak = Peak();
#endif

#ifdef USE_POTENTIOMETER
Potentiometer potentiometer = Potentiometer();
#endif

#ifdef USE_TOUCH
Touch touch = Touch(&tft, &ui);
#endif

MIDI_CREATE_DEFAULT_INSTANCE();

#ifdef USE_ROTARY
void rotary_onValueChanged() {
  MIDI.sendControlChange(currentControl, rotary.getEncoderValue(), currentChannel);
  ui.toggleShouldUpdate();
}
#endif

#ifdef USE_TOUCH
static void btnL_pressAction(void) {
  if (ui.btnL.justReleased()) {
    currentControl = clamp(currentControl--, MIN_CONTROL, MAX_CONTROL, true);
    ui.toggleShouldUpdate();
  }
}

static void btnR_pressAction(void) {
  if (ui.btnR.justReleased()) {
    currentControl = clamp(currentControl++, MIN_CONTROL, MAX_CONTROL, true);
    ui.toggleShouldUpdate();
  }
}
#endif

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

#ifdef USE_TOUCH
  touch.calibrate();
#endif

#ifdef USE_UI
  ui.init();
  ui.update();
#ifdef USE_TOUCH
  ui.initButtons(btnL_pressAction, btnR_pressAction);
#endif
#endif

  MIDI.begin(1);

#ifdef USE_ROTARY
  rotary.init();
#endif
}

void loop() {
#ifdef USE_ROTARY
  rotary.loop(rotary_onValueChanged);
  currentValue = rotary.getEncoderValue();
#endif

#ifdef USE_TOUCH
  touch.loop();
#endif

#ifdef USE_POTENTIOMETER
  currentValue = potentiometer.readMappedValue(MIN_VALUE, MAX_VALUE);
#endif

#ifdef USE_UI
  ui.update(currentValue, currentControl, currentChannel);
#endif
}

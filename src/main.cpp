#include <MIDI.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include <peak.hpp>
#include <potentiometer.hpp>
#include <rotary.hpp>
#include <touch.hpp>
#include <ui.hpp>

int currentOutValue = MIN_VALUE;
int currentOutControl = MIN_CONTROL;
int currentOutChannel = MIN_CHANNEL;

int currentInValue = MIN_VALUE;
int currentInChannel = MIN_CHANNEL;

TFT_eSPI tft = TFT_eSPI();

MIDI_CREATE_DEFAULT_INSTANCE();

#ifdef USE_UI
Ui ui = Ui(&tft, &currentOutControl, &currentOutChannel);
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

#ifdef USE_ROTARY
void rotary_onValueChanged() { ui.toggleShouldUpdate(); }
#endif

#ifdef USE_TOUCH
static void btnL_pressAction(void) {
  if (ui.btnL.justReleased()) {
    currentOutControl = clamp(currentOutControl--, MIN_CONTROL, MAX_CONTROL, true);
    ui.toggleShouldUpdate();
  }
}

static void btnR_pressAction(void) {
  if (ui.btnR.justReleased()) {
    currentOutControl = clamp(currentOutControl++, MIN_CONTROL, MAX_CONTROL, true);
    ui.toggleShouldUpdate();
  }
}
#endif

void handleNoteOn(byte channel, byte pitch, byte velocity) { currentInValue = velocity; }

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

  MIDI.begin(currentInChannel);
  MIDI.setHandleNoteOn(handleNoteOn);

#ifdef USE_ROTARY
  rotary.init();
#endif
}

void loop() {
  MIDI.read();

#ifdef USE_ROTARY
  rotary.loop(rotary_onValueChanged);
  currentOutValue = rotary.getEncoderValue();
#endif

#ifdef USE_TOUCH
  touch.loop();
#endif

#ifdef USE_POTENTIOMETER
  currentOutValue = potentiometer.readMappedValue(MIN_VALUE, MAX_VALUE);
#endif

#ifdef USE_PEAK
  peak.loop(currentInValue);
#endif

#ifdef USE_UI
  ui.update(currentOutValue, currentOutControl, currentOutChannel);
#endif

  MIDI.sendControlChange(currentOutControl, currentOutValue, currentOutChannel);
}

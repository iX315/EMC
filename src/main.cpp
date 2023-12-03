#include <MIDI.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <HardwareSerial.h>

#include <peak.hpp>
#include <potentiometer.hpp>
#include <rotary.hpp>
#include <touch.hpp>
#include <ui.hpp>

int lastOutValue = MIN_VALUE;
int currentOutValue = MIN_VALUE;
int currentOutControl = MIN_CONTROL;
int currentOutChannel = MIN_CHANNEL;

int currentInValue = MIN_VALUE;
int currentInChannel = MIN_CHANNEL;

TFT_eSPI tft = TFT_eSPI();

struct CustomBaud : public midi::DefaultSettings{
    static const long BaudRate = 115200; // Baud rate for hairless
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI2, CustomBaud);

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

void handleAfterTouchChannel(byte channel, byte value) {
#ifdef DEBUG_MIDI
  Serial.println("AfterTouchChannel: " + String(channel) + ", " + String(value));
#endif
  currentInValue = value;
}

void setup() {
  MIDI2.setHandleAfterTouchChannel(handleAfterTouchChannel);
  MIDI2.begin(MIDI_CHANNEL_OMNI); // read all incoming messages

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

#ifdef USE_ROTARY
  rotary.init();
#endif
}

void loop() {
  MIDI2.read();

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

  if (currentOutValue != lastOutValue) {
    lastOutValue = currentOutValue;
    MIDI2.sendPitchBend(currentOutValue, currentOutChannel);
  }
}

//#include "tft.h"
#include "controls.h"
#include "rotary.h"
#include "AiEsp32RotaryEncoder.h"

#include <SoftwareSerial.h>
#include <MIDI.h>
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

MIDI_CREATE_DEFAULT_INSTANCE();

TFT_eSPI tft = TFT_eSPI();

int currentControl = MIN_CONTROL;
int currentChannel = MIN_CHANNEL;

int newPosition = 15;
int oldPosition = 0;

ButtonWidget btnL = ButtonWidget(&tft);
ButtonWidget btnOk = ButtonWidget(&tft);
ButtonWidget btnR = ButtonWidget(&tft);

ButtonWidget* btn[] = {&btnL , &btnR};;
uint8_t buttonCount = sizeof(btn) / sizeof(btn[0]);

int constrainValue(int value, int minValue, int maxValue) {
  if (value < minValue) {
    return maxValue;
  } else if (value > maxValue) {
    return minValue;
  } else {
    return value;
  }
}

void uiUpdate(int Value = 0, int Control = 0, int Channel = 0) {
  tft.setCursor(0, 0);
  tft.setTextSize(4);
  tft.setTextColor(TFT_DARKGREEN, TFT_BLACK);
  tft.print("Value: "); tft.println(Value);
  tft.setTextSize(2);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.print("Control: "); tft.println(Control);
  tft.print("Channel: "); tft.println(Channel);
}

void btnL_pressAction(void) {
  if (btnL.justReleased()) {
    currentControl = constrainValue(currentControl--, MIN_CONTROL, MAX_CONTROL);
    uiUpdate(newPosition, currentControl, currentChannel);
  }
}

void btnR_pressAction(void) {
  if (btnR.justReleased()) {
    currentControl = constrainValue(currentControl++, MIN_CONTROL, MAX_CONTROL);
    uiUpdate(newPosition, currentControl, currentChannel);
  }
}

void initBtns() {
  btnL.initButtonUL(0, 200, 40, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, "<", 3);
  btnL.drawButton(false);
  btnL.setPressAction(btnL_pressAction);

  btnOk.initButtonUL(60, 200, 120, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, "OK", 3);
  btnOk.drawButton(false);

  btnR.initButtonUL(200, 200, 40, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, ">", 3);
  btnR.drawButton(false);
  btnR.setPressAction(btnR_pressAction);
}

void initTFT() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
}

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

void initRotary() {
  // initialize rotary encoder
  rotaryEncoder.begin();

  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), readEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_B_PIN), readEncoderISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_BUTTON_PIN), rotary_onButtonClick, RISING);

  // max, min, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.setBoundaries(MIN_VALUE, MAX_VALUE, true); 
  // set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  rotaryEncoder.setAcceleration(0);
}

void rotary_loop() {
  // dont do anything unless value changed
  if (!rotaryEncoder.encoderChanged()) {
    return;
  } else {
    newPosition = rotaryEncoder.readEncoder();

    MIDI.sendControlChange(currentControl, newPosition, currentChannel);

    uiUpdate(newPosition, currentControl, currentChannel);
  }
}

void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!LittleFS.begin()) {
    Serial.println("Formating file system");
    LittleFS.format();
    LittleFS.begin();
  }

  // check if calibration file exists and size is correct
  if (LittleFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL) {
      // Delete if we want to re-calibrate
      LittleFS.remove(CALIBRATION_FILE);
    } else {
      File f = LittleFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = LittleFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

void touchLoop() {
  static uint32_t scanTime = millis();
  uint16_t t_x = 9999, t_y = 9999; // To store the touch coordinates

  // Scan keys every 50ms at most
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    scanTime = millis();
    for (uint8_t b = 0; b < buttonCount; b++) {
      if (pressed) {
        if (btn[b]->contains(t_x, t_y)) {
          btn[b]->press(true);
          btn[b]->pressAction();
        }
      }
      else {
        btn[b]->press(false);
        btn[b]->releaseAction();
      }
    }
  }
}

void setup() {
  initTFT();
  MIDI.begin(1);
  Serial.begin(115200);

  initRotary();
  uiUpdate();
  initBtns();
  touch_calibrate();
}

void loop() {
  rotary_loop();
  touchLoop();
}

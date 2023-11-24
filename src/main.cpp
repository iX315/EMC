#include <rotary.h>
#include <ui.h>

#include <SoftwareSerial.h>
#include <MIDI.h>
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

TFT_eSPI tft = TFT_eSPI();

Ui ui = Ui(&tft);
Rotary rotary = Rotary();

MIDI_CREATE_DEFAULT_INSTANCE();

int currentControl = MIN_CONTROL;
int currentChannel = MIN_CHANNEL;

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

void btnL_pressAction(void) {
  if (btnL.justReleased()) {
    currentControl = constrainValue(currentControl--, MIN_CONTROL, MAX_CONTROL);
    ui.toggleShouldUpdate();
  }
}

void btnR_pressAction(void) {
  if (btnR.justReleased()) {
    currentControl = constrainValue(currentControl++, MIN_CONTROL, MAX_CONTROL);
    ui.toggleShouldUpdate();
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

void rotary_onValueChanged() {
  MIDI.sendControlChange(currentControl, rotary.getEncoderValue(), currentChannel);
  ui.toggleShouldUpdate();
}

void setup() {
  Serial.println("Hello!");
  ui.init();
  MIDI.begin(1);
  Serial.begin(115200);

  rotary.init();

  ui.update();
  initBtns();
  touch_calibrate();
}

void loop() {
  rotary.loop(rotary_onValueChanged);
  touchLoop();
  ui.update(rotary.getEncoderValue(), currentControl, currentChannel);
}

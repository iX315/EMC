#include "touch.hpp"

Touch::Touch(TFT_eSPI *tft, Ui *ui) {
  _tft = tft;
  _ui = ui;
};

void Touch::calibrate() {
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
    _tft->setTouch(calData);
  } else {
    // data not valid so recalibrate
    _tft->fillScreen(TFT_BLACK);
    _tft->setCursor(20, 0);
    _tft->setTextFont(2);
    _tft->setTextSize(1);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);

    _tft->println("Touch corners as indicated");

    _tft->setTextFont(1);
    _tft->println();

    if (REPEAT_CAL) {
      _tft->setTextColor(TFT_RED, TFT_BLACK);
      _tft->println("Set REPEAT_CAL to false to stop this running again!");
    }

    _tft->calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
    _tft->println("Calibration complete!");

    // store data
    File f = LittleFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

void Touch::loop() {
  static uint32_t scanTime = millis();
  uint16_t t_x = 9999, t_y = 9999; // To store the touch coordinates

  // Scan keys every 50ms at most
  if (millis() - scanTime >= 50) {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = _tft->getTouch(&t_x, &t_y);
    scanTime = millis();
    for (uint8_t b = 0; b < _ui->getButtonCount(); b++) {
      ButtonWidget Btn = _ui->getButton(b);
      if (pressed) {
        if (Btn.contains(t_x, t_y)) {
          Btn.press(true);
          Btn.pressAction();
        }
      } else {
        Btn.press(false);
        Btn.releaseAction();
      }
    }
  }
}

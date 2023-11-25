#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <UI.hpp>

#ifndef TOUCH_H
#define TOUCH_H

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

class Touch {
public:
  Touch(TFT_eSPI *tft, Ui *ui);
  void calibrate();
  void loop();

private:
  TFT_eSPI *_tft;
  Ui *_ui;
};

#endif // TOUCH_H
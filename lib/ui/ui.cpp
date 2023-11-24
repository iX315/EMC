#include "ui.h"
#include <TFT_eWidget.h>

Ui::Ui(TFT_eSPI *tft, int *currentControl, int *currentChannel) {
    _tft = tft;
    _shouldUpdate = false;

    _currentControl = currentControl;
    _currentChannel = currentChannel;

    _btnL = ButtonWidget(&tft);
    _btnOk = ButtonWidget(&tft);
    _btnR = ButtonWidget(&tft);

    _btn[] = {&_btnL , &_btnR};;
    _buttonCount = sizeof(_btn) / sizeof(_btn[0]);
}

void Ui::toggleShouldUpdate() {
    _shouldUpdate = !_shouldUpdate;
}

void Ui::init() {
    _tft->init();
    _tft->setRotation(0);
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextColor(TFT_WHITE);
    _tft->setCursor(0, 0);
}

void Ui::update(int Value, int Control, int Channel) {
    if (!_shouldUpdate) {
        return;
    }
    _tft->setCursor(0, 0);
    _tft->setTextSize(4);
    _tft->setTextColor(TFT_DARKGREEN, TFT_BLACK);
    _tft->print("Value: "); _tft->println(Value);
    _tft->setTextSize(2);
    _tft->setTextColor(TFT_ORANGE, TFT_BLACK);
    _tft->print("Control: "); _tft->println(Control);
    _tft->print("Channel: "); _tft->println(Channel);
    toggleShouldUpdate();
}

int constrainValue(int value, int minValue, int maxValue) {
  if (value < minValue) {
    return maxValue;
  } else if (value > maxValue) {
    return minValue;
  } else {
    return value;
  }
}

void Ui::btnL_pressAction(void) {
  if (_btnL.justReleased()) {
    _currentControl = constrainValue(_currentControl--, MIN_CONTROL, MAX_CONTROL);
    toggleShouldUpdate();
  }
}

void Ui::btnR_pressAction(void) {
  if (_btnR.justReleased()) {
    _currentControl = constrainValue(_currentControl++, MIN_CONTROL, MAX_CONTROL);
    toggleShouldUpdate();
  }
}

void Ui::initButtons() {
  _btnL.initButtonUL(0, 200, 40, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, "<", 3);
  _btnL.drawButton(false);
  _btnL.setPressAction(btnL_pressAction);

  _btnOk.initButtonUL(60, 200, 120, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, "OK", 3);
  _btnOk.drawButton(false);

  _btnR.initButtonUL(200, 200, 40, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, ">", 3);
  _btnR.drawButton(false);
  _btnR.setPressAction(btnR_pressAction);
}
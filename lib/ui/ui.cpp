#include "ui.hpp"

Ui::Ui(TFT_eSPI *tft, int *currentOutControl, int *currentOutChannel) : btnL(tft), btnOk(tft), btnR(tft) {
  _tft = tft;
  _shouldUpdate = false;
  _buttonCount = 3;
}

void Ui::toggleShouldUpdate() { _shouldUpdate = !_shouldUpdate; }

void Ui::init() {
  _tft->init();
  _tft->setRotation(0);
  _tft->fillScreen(TFT_BLACK);
  _tft->setTextColor(TFT_WHITE);
  _tft->setCursor(0, 0);
  update(0, 0, 0);
}

String newLinePrint(int value) {
  if (value < 10) {
      return String(value) + "  ";
    } else if (value < 100) {
      return String(value) + " ";
    }
    return String(value);
}

void Ui::update(int Value, int Control, int Channel) {
  if (!_shouldUpdate) {
    return;
  }
  _tft->setCursor(0, 0);
  _tft->setTextSize(4);
  _tft->setTextColor(TFT_DARKGREEN, TFT_BLACK);
  _tft->println("Value: " + newLinePrint(Value));
  _tft->setTextSize(3);
  _tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  _tft->println("Control: " + newLinePrint(Control));
  _tft->println("Channel: " + newLinePrint(Channel));
  toggleShouldUpdate();
}

void Ui::initButtons(actionCallback btnL_pressAction, actionCallback btnR_pressAction) {
  btnL.initButtonUL(0, 200, 40, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, "<", 3);
  btnL.drawButton(false);
  btnL.setPressAction(btnL_pressAction);

  btnOk.initButtonUL(60, 200, 120, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, "OK", 3);
  btnOk.drawButton(false);

  btnR.initButtonUL(200, 200, 40, 120, TFT_WHITE, TFT_BLACK, TFT_WHITE, ">", 3);
  btnR.drawButton(false);
  btnR.setPressAction(btnR_pressAction);
}
#include "../constants.hpp"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <utils.hpp>

#ifndef UI_H
#define UI_H

class Ui {
public:
  Ui(TFT_eSPI *tft, int *currentOutControl, int *currentOutChannel);

  void toggleShouldUpdate();
  void init();
  void update(int Value = 0, int Control = 0, int Channel = 0);
  void initButtons(actionCallback btnL_pressAction, actionCallback btnR_pressAction);

  ButtonWidget btnL;
  ButtonWidget btnOk;
  ButtonWidget btnR;

  int getButtonCount() { return _buttonCount; };

  ButtonWidget *getButton(int index) {
    if (index < 0 || index >= _buttonCount) {
      return nullptr;
    }
    return &_buttons[index];
  };

private:
  TFT_eSPI *_tft;
  bool _shouldUpdate;

  ButtonWidget _buttons[3] = {btnL, btnOk, btnR};
  uint8_t _buttonCount = 3;
};

#endif // UI_H
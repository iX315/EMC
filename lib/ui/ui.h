#include <SPI.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include "../constants.h"
#include "../utils.h"

#ifndef UI_H
#define UI_H

class Ui {
public:
    Ui(TFT_eSPI *tft, int *currentControl, int *currentChannel);

    void toggleShouldUpdate();
    void init();
    void update(int Value = 0, int Control = 0, int Channel = 0);
    void initButtons(actionCallback btnL_pressAction, actionCallback btnR_pressAction);

    ButtonWidget btnL;
    ButtonWidget btnOk;
    ButtonWidget btnR;

    void decreaseControl();
    void increaseControl();

    int buttonCount() { return _buttonCount; };

    ButtonWidget buttons[3] = { btnL, btnOk, btnR };

private:
    TFT_eSPI *_tft;
    bool _shouldUpdate;
    int _currentControl;
    int _currentChannel;

    uint8_t _buttonCount = 3;
};

#endif // UI_H
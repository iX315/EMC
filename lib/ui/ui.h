#ifndef UI_H
#define UI_H

#include "tft.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>

class Ui {
public:
    Ui(TFT_eSPI *tft, int *currentControl, int *currentChannel);

    void toggleShouldUpdate();
    void init();
    void update(int Value = 0, int Control = 0, int Channel = 0);
    void initButtons();
    void btnL_pressAction(void);
    void btnR_pressAction(void);

    actionCallback callback = noop;

private:
    TFT_eSPI *_tft;
    bool _shouldUpdate;
    int *_currentControl;
    int *_currentChannel;
    ButtonWidget _btnL;
    ButtonWidget _btnOk;
    ButtonWidget _btnR;
    ButtonWidget* _btn;
    uint8_t _buttonCount;
};

#endif // UI_H
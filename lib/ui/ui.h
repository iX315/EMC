#ifndef UI_H
#define UI_H

#include "tft.h"
#include <SPI.h>
#include <TFT_eSPI.h>

class Ui {
public:
    Ui(TFT_eSPI *tft);

    void toggleShouldUpdate();
    void init();
    void update(int Value = 0, int Control = 0, int Channel = 0);
private:
    TFT_eSPI *_tft;
    bool _shouldUpdate;
};

#endif // UI_H
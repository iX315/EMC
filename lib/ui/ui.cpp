#include "ui.h"

Ui::Ui(TFT_eSPI *tft) {
    _tft = tft;
    _shouldUpdate = false;
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
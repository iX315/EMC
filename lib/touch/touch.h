#ifndef TOUCH_H
#define TOUCH_H

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

class Touch {
public:
    Touch();
    void calibrate();
    void loop();
private:
    // Private members and methods
};

#endif // TOUCH_H
#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <ASTCanLib.h>

class Sensor {
protected:
    uint8_t pin;
    void (*interruptFun)();     // optional

public:
    Sensor() : pin(0), interruptFun(nullptr) {}

    virtual void init(uint8_t pin_num, void (*intFun)() = nullptr) {
        pin = pin_num;
        pinMode(pin, INPUT);

        if (intFun) {
            interruptFun = intFun;
            attachInterrupt(digitalPinToInterrupt(pin), intFun, RISING);
        }
    }

    virtual int16_t calculate() = 0;  // Sensor-specific calculation logic

    virtual void intHandler() {}  // Optional interrupt handling
};

#endif

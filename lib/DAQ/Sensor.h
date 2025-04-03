#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <ASTCanLib.h>
#include "MovingSum.h"
#include "SensorTypes.h"

#define ERROR false
#define NO_ERROR true

class Sensor {
protected:
    uint8_t pin;
    void (*interruptFun)();     // optional
    MovingSum msum;

public:
    Sensor() : pin(0), interruptFun(nullptr) {}

    virtual void init(uint8_t pin_num, void (*intFun)() = nullptr, uint8_t num_readings = 1) {
        pin = pin_num;
        pinMode(pin, INPUT);

        if (intFun) {
            interruptFun = intFun;
            attachInterrupt(digitalPinToInterrupt(pin), intFun, RISING);
        }
        MovingSum_init(&msum, num_readings);
    }

    uint16_t readRaw() { return analogRead(pin); }

    /*  
        Sensor-sepcific calculation logic 
        Returns 1 for success, 0 for error
        Make sure to use the correct sensor type defined in SensorTypes.h
    */
    virtual bool calculate(void* result) = 0;

    virtual void intHandler() {}  // Optional interrupt handling
};

#endif

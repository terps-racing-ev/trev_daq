#ifndef FLOWMETER_H
#define FLOWMETER_H

#include <Arduino.h>
#include "CANManager.h"

class FlowMeterSensor {

private:
    uint8_t pin;

    volatile uint32_t count;
    uint32_t rate;

    CANManager msg;

public:
    void inc();
    FlowMeterSensor() : count(0), rate(0) {}
    void init(uint8_t pin_num, void(*intFun)(), uint16_t id);
    void tx();
};

#endif
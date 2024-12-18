#ifndef BRAKEPRESSURE_H
#define BRAKEPRESSURE_H

#include <Arduino.h>
#include "CANManager.h"

class BrakePressureSensor {

private:
    uint8_t pin;

    CANManager msg;

public:
    BrakePressureSensor();
    void init(uint8_t pin_num, uint16_t id);
    void tx();
};

#endif
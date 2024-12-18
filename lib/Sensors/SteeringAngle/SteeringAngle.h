#ifndef STEERINGANGLE_H
#define STEERINGANGLE_H

#include <Arduino.h>
#include "CANManager.h"

class SteeringAngleSensor {

private:
    uint8_t pin;
    int potValue;
    float angle;

    CANManager msg;

public:
    SteeringAngleSensor() : potValue(0), angle(0) {}
    void init(uint8_t pin_num, uint16_t id);
    void tx();
};

#endif
#ifndef LINEARPOT_H
#define LINEARPOT_H

#include <Arduino.h>
#include "CANManager.h"

#define SIZE 5
#define MAX_RANGE 50

class LinearPotSensor {

private:
    uint8_t pin;

    int idx;
    int sum;
    int readings[SIZE];
    int avg;

    CANManager msg;

public:
    LinearPotSensor() : idx(0), sum(0), avg(0) {}
    void init(uint8_t pin_num, uint16_t id);
    void tx();
};

#endif

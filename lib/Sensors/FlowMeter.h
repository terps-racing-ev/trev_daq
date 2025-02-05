#ifndef FLOW_METER_H
#define FLOW_METER_H

#include <Arduino.h>
#include "Sensor.h"

class FlowMeter : public Sensor {

private:
    volatile uint32_t count;
    float rate;

public:
    FlowMeter() : count(0), rate(0) {}

    void intHandler() override {
        count++;
    }

    float calculate() override {
        EIMSK &= ~(1 << digitalPinToInterrupt(pin));

        rate = (count / 7.5); //Pulse frequency / 7.5Q, = flow rate in L/minute
        
        EIMSK |= (1 << digitalPinToInterrupt(pin));
        
        count = 0;
        
        tx(&rate, sizeof(rate));
        return rate;
    }
};

#endif

#ifndef FLOW_METER_H
#define FLOW_METER_H

#include <Arduino.h>
#include "Sensor.h"

class FlowMeter : public Sensor {

private:
    volatile uint32_t count;

public:
    FlowMeter() : count(0) {}

    void intHandler() override {
        count++;
    }

    int16_t calculate() override {
        EIMSK &= ~(1 << digitalPinToInterrupt(pin));
        int16_t flow_rate = (count / 75) * 100; //Pulse frequency / 7.5Q, = flow rate in L/minute
        EIMSK |= (1 << digitalPinToInterrupt(pin));
        
        count = 0;
        return flow_rate; // Flow rate in L/minute * 10
    }
};

#endif

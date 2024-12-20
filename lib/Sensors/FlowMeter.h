#ifndef FLOW_METER_H
#define FLOW_METER_H

#include <Arduino.h>
#include "Sensor.h"

class FlowMeter : public Sensor {

private:
    volatile uint32_t count;
    uint32_t rate;

public:
    FlowMeter() : count(0), rate(0) {}

    void intHandler() override {
        count++;
    }

    void calculate() override {
        EIMSK &= ~(1 << digitalPinToInterrupt(pin));

        rate = (count * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour
        Serial.println (rate);
        
        tx(&rate, sizeof(rate));

        EIMSK |= (1 << digitalPinToInterrupt(pin));
    }
};

#endif

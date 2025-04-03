#ifndef FLOW_METER_H
#define FLOW_METER_H

#include <Arduino.h>
#include "Sensor.h"

class FlowMeter : public Sensor {

private:
    /*
    Some realistic times between pulses:
    1L/min ~133,333us
    30L/min ~4,500us

    */
    // FLOW_CONST = (1/7.5) * (us in s) * 10x multiplier
    static constexpr uint32_t FLOW_CONST = 1333333;

    static constexpr uint32_t TIMEOUT = 1000000;

    volatile uint32_t lastPulseTime;
    volatile uint32_t delta;
    volatile bool newPulse;

public:
    FlowMeter() : lastPulseTime(0), delta(0), newPulse(0) {}

    void intHandler() override {
        uint32_t now = micros();
        delta = now-lastPulseTime;
        lastPulseTime = now;
        newPulse = true;
    }

    boolean calculate(void* result) override {
        flow_type* flow_rate = static_cast<flow_type*>(result);
        if (flow_rate == nullptr) return ERROR;  // Error: result pointer is null

        EIMSK &= ~(1 << digitalPinToInterrupt(pin));

        // If pulse takes too long, reset moving sum to avoid keeping old values
        if (micros() - lastPulseTime > TIMEOUT) {  
            for(uint8_t i = 0; i < msum.num_readings; i++){
                msum.readings[i] = 0;
            }
            msum.sum = 0;
            msum.count = 0;
        }
        // Only update moving sum if a pulse has arrived
        if(newPulse) {
            MovingSum_update(&msum, delta);
            newPulse = false;
        }

        EIMSK |= (1 << digitalPinToInterrupt(pin));

        *flow_rate = msum.sum > 0 ? 
            (uint32_t)(FLOW_CONST * msum.count) / msum.sum : 0;
        return NO_ERROR; // Moving average of Flow rate in L/min x 10
    }
};

#endif

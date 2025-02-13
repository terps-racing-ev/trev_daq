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
    // FLOW_CONST = 1/7.5 * 10^6 * 10
    static constexpr uint32_t FLOW_CONST = 1333333;

    static constexpr uint8_t NUM_READINGS = 4;
    static constexpr uint32_t TIMEOUT = 5000000;

    volatile uint32_t lastPulseTime;
    volatile uint32_t pulseTimes[NUM_READINGS];
    volatile uint8_t readIdx;
    volatile uint32_t sum;


public:
    FlowMeter() : lastPulseTime(0), readIdx(0), sum(0) {
        // memset not allowed with volatile
        for(uint8_t i = 0; i < NUM_READINGS; i++){
            pulseTimes[i] = 0;
        }
    }

    void intHandler() override {
        uint32_t now = micros();
        uint32_t delta = now-lastPulseTime;
        sum = sum - pulseTimes[readIdx];
        pulseTimes[readIdx] = delta;
        sum = sum + delta;
        readIdx = (readIdx + 1) % NUM_READINGS;
        lastPulseTime = now;
    }

    int16_t calculate() override {
        EIMSK &= ~(1 << digitalPinToInterrupt(pin));

        if (micros() - lastPulseTime > TIMEOUT) {  
            for(uint8_t i = 0; i < NUM_READINGS; i++){
                pulseTimes[i] = 0;
            }
            sum = 0;
        }
        uint32_t flowRate = sum == 0 ? 0 : 
            (FLOW_CONST * NUM_READINGS) / sum;

        EIMSK |= (1 << digitalPinToInterrupt(pin));
        return flowRate; // Moving average of Flow rate in L/min x 10
    }
};

#endif

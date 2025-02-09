#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>
#include "Sensor.h"

class WheelSpeed : public Sensor {

private:
    static constexpr uint16_t ROLLING_RADIUS = 900;  // Hundredths of inches
    static constexpr uint8_t SPOKES = 16;
    /*
    Some realistic times between pulses:
    1mph ~200,000us
    40mph ~5,000us
    100mph ~2,000us
    */    
    // WSPD_CONST = (2*pi)(microseconds per hour) / (hundredths of inches per mile) = 3569.992
    static constexpr uint16_t WSPD_CONST = 3570;
    static constexpr uint8_t NUM_READINGS = 4;
    static constexpr uint32_t TIMEOUT = 5000000;

    volatile uint32_t lastPulseTime;

    uint32_t pulseTimes[NUM_READINGS];  
    uint8_t readIdx;
    uint32_t sum;

public:
    WheelSpeed() : lastPulseTime(0), readIdx(0), sum(0) {
        memset(pulseTimes, 0, sizeof(pulseTimes));
    }
    
    void intHandler() override {
        uint32_t now = micros();
        uint32_t time = now-lastPulseTime;
        sum = sum - pulseTimes[readIdx];
        pulseTimes[readIdx] = time;
        sum = sum + time;
        readIdx = (readIdx + 1) % NUM_READINGS;
        lastPulseTime = now;
    }
    
    int16_t calculate() override {
        
        if (micros() - lastPulseTime > TIMEOUT) {  
            memset(pulseTimes, 0, sizeof(pulseTimes));
            sum = 0;
        }

        EIMSK &= ~(1 << digitalPinToInterrupt(pin));
        uint32_t mph32 = sum == 0 ? 0 : (WSPD_CONST * ROLLING_RADIUS * 100) / (sum * SPOKES);
        EIMSK |= (1 << digitalPinToInterrupt(pin));

        uint16_t mph16 = static_cast<uint16_t>(mph32);
        return mph16;
    }
};

#endif

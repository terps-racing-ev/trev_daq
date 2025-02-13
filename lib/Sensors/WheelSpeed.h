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
    static constexpr uint32_t WSPD_CONST = 3570;

    static constexpr uint8_t NUM_READINGS = 4;
    static constexpr uint32_t TIMEOUT = 5000000;

    volatile uint32_t lastPulseTime;
    volatile uint32_t pulseTimes[NUM_READINGS];  
    volatile uint8_t readIdx;
    volatile uint32_t sum;

public:
    WheelSpeed() : lastPulseTime(0), readIdx(0), sum(0) {
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
        uint32_t mph = sum == 0 ? 0 : 
            (WSPD_CONST * ROLLING_RADIUS * NUM_READINGS * 100) / 
            (sum * SPOKES);
        
        EIMSK |= (1 << digitalPinToInterrupt(pin));
        return mph; // Moving average of Speed in MPH x 100
    }
};

#endif

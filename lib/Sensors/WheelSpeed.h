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
    // WSPD_CONST = (2*pi)(us per hour) / (hundredths of inches per mile) = 3569.992
    static constexpr uint32_t WSPD_CONST = 3570;

    static constexpr uint32_t TIMEOUT = 1000000;

    volatile uint32_t lastPulseTime;
    volatile uint32_t delta;
    volatile bool newPulse;

public:
    WheelSpeed() : lastPulseTime(0), delta(0), newPulse(false) {}
    
    void intHandler() override {
        uint32_t now = micros();
        delta = now-lastPulseTime;
        lastPulseTime = now;
        newPulse = true;
    }
    
    boolean calculate(void* result) override {
        wsp_type* mph_scaled = static_cast<wsp_type*>(result);
        if (mph_scaled == nullptr) return ERROR;  // Error: result pointer is null

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
        if(newPulse){
            MovingSum_update(&msum, delta);
            newPulse = false;
        }

        EIMSK |= (1 << digitalPinToInterrupt(pin));

        // Moving average of Speed in MPH x 100
        *mph_scaled = msum.sum > 0 ? 
            (uint64_t)(WSPD_CONST * ROLLING_RADIUS * msum.count * 100) / (msum.sum * SPOKES) : 0;
        return NO_ERROR;
    }
};

#endif

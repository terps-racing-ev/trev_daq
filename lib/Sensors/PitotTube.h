#ifndef PITOT_TUBE_H
#define PITOT_TUBE_H

#include <Arduino.h>
#include "Sensor.h"

class PitotTube : public Sensor {

private:
    static constexpr float AIR_DENSITY = 1.225;

public:
    PitotTube() { }

    boolean calculate(void* result) override {
        pitot_type* vel_filtered = static_cast<pitot_type*>(result);
        if (vel_filtered == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t mV = map(analogRead(pin), 0, 1023, 0, 5000);
        int16_t Pa = mV - 2740; //voltage to pressure differential conversion
 
        float vel = sqrt(abs(2.0 * Pa / AIR_DENSITY)) * 2.23694;
        uint16_t vel_scaled = static_cast<int16_t>(vel * 100);

        MovingSum_update(&msum, vel_scaled);

        *vel_filtered = msum.count < 0 ? msum.sum / msum.count : 0;  // Moving average of Velocity in MPH x 100
        return NO_ERROR;
    }
};

#endif

#ifndef PITOT_TUBE_H
#define PITOT_TUBE_H

#include <Arduino.h>
#include "Sensor.h"

class PitotTube : public Sensor {

private:
    static constexpr uint16_t NUM_READINGS = 20;
    static constexpr float AIR_DENSITY = 1.225;

    uint16_t readings[NUM_READINGS];   // stores velocity values based on numReadings
    uint16_t readIdx;                  // index of the current reading
    uint32_t sum;                      // sum of the last numReadings 

public:
    PitotTube() : readIdx(0), sum(0) {
        memset(readings, 0, sizeof(readings));
    }

    int16_t calculate() override {
        int16_t mV = map(analogRead(pin), 0, 1023, 0, 5000);
        int16_t Pa = mV - 2740;     //voltage to pressure differential conversion
 
        float vel = sqrt(abs(2.0 * Pa / AIR_DENSITY)) * 2.23694;
        int16_t vel_scaled = static_cast<int16_t>(vel * 100);

        sum -= readings[readIdx];
        readings[readIdx] = vel_scaled;
        sum += readings[readIdx];
        readIdx = (readIdx + 1) % NUM_READINGS;

        int16_t avg = sum / NUM_READINGS;
        return Pa; // Moving average of Velocity in MPH x 100
    }
};

#endif

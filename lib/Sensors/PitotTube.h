#ifndef PITOT_TUBE_H
#define PITOT_TUBE_H

#include <Arduino.h>
#include "Sensor.h"

class PitotTube : public Sensor {

private:
    static constexpr uint16_t NUM_READINGS = 20;

    uint16_t readings[NUM_READINGS];   // stores velocity values based on numReadings
    uint16_t readIdx;                  // index of the current reading
    uint32_t sum;                      // sum of the last numReadings 

public:
    PitotTube() : readIdx(0), sum(0) {
        memset(readings, 0, sizeof(readings));
    }

    int16_t calculate() override {
        int16_t mV = analogRead(pin) * 5000 / 1023;
        int16_t Pa = mV - 2740;     //voltage to pressure differential conversion
 
        float vel = sqrt(abs(2.0 * Pa / 1.225)) * 2.23694;  //find velocity in mph
        uint16_t vel_scaled = static_cast<uint16_t>(vel * 100);

        //implement moving average
        // Subtract the last reading from the total
        sum -= readings[readIdx];
        // Add reading 
        readings[readIdx] = vel_scaled;
        sum += readings[readIdx];
        // Next array position
        readIdx = (readIdx + 1) % NUM_READINGS;

        int16_t avg = sum / NUM_READINGS;
        return avg; // Moving average of Velocity in MPH * 100
    }
};

#endif

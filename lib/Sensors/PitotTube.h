#ifndef PITOT_TUBE_H
#define PITOT_TUBE_H

#include <Arduino.h>
#include "Sensor.h"

class PitotTube : public Sensor {

#define NUM_READINGS 20

private:
    float readings[NUM_READINGS];   // stores velocity values based on numReadings
    int readIdx = 0;                // index of the current reading
    float sum = 0;                  // sum of the last numReadings 
    float avg = 0;                  // moving average of the readings

public:
    PitotTube() : readIdx(0), sum(0), avg(0) {
        memset(readings, 0, sizeof(readings));
    }

    // moving average
    float calculate() override {
        float V = mapFloat(analogRead(pin), 0, 1023, 0, 5);
        float kPa = V - 2.74;     //voltage ratio to pressure differential conversion
 
        float Pa = 1000 * kPa;      //convert to pascals
        float vel = sqrt(abs(2*(Pa) / (1.225)))*2.23694;  //find velocity in mph

        //implement moving average
        // Subtract the last reading from the total
        sum -= readings[readIdx];
        // Add reading 
        readings[readIdx] = vel;
        sum += readings[readIdx];
        // Next array position
        readIdx = (readIdx + 1) % NUM_READINGS;

        avg = sum / NUM_READINGS;
        tx(&avg, sizeof(avg));
        return avg;
    }
};

#endif

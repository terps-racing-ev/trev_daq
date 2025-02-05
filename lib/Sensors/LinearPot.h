#ifndef LINEAR_POT_H
#define LINEAR_POT_H

#include <Arduino.h>
#include "Sensor.h"

#define NUM_READINGS 5
#define MAX_RANGE 50

class LinearPot : public Sensor {

private:
    int readIdx;
    float sum;
    float readings[NUM_READINGS];
    float avg;

public:
    LinearPot() : readIdx(0), sum(0), avg(0) {
        memset(readings, 0, sizeof(readings));
    }

    // moving average
    float calculate() override {
        float dist = mapFloat(analogRead(pin), 0, 1023, 0, MAX_RANGE);
        sum = sum - readings[readIdx];
        readings[readIdx] = dist;
        sum = sum + dist;
        readIdx = (readIdx + 1) % NUM_READINGS;
        avg = sum / NUM_READINGS;
        tx(&avg, sizeof(avg));
        return avg;
    }
};

#endif

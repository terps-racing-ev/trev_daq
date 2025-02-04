#ifndef LINEAR_POT_H
#define LINEAR_POT_H

#include <Arduino.h>
#include "Sensor.h"

#define SIZE 5
#define MAX_RANGE 50

class LinearPot : public Sensor {

private:
    int idx;
    int sum;
    int readings[SIZE];
    int avg;

public:
    LinearPot() : idx(0), sum(0), avg(0) {}

    void calculate() override {
        double a = analogRead(pin);
        double x = MAX_RANGE * a/1023;
        sum = sum - readings[idx];
        readings[idx] = x;
        sum = sum + x;
        idx = (idx + 1) % SIZE;
        avg = sum / SIZE;
        Serial.println(avg);
        tx(&avg, sizeof(avg));
    }
};

#endif

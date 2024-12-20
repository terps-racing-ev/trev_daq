#ifndef STEERING_ANGLE_H
#define STEERING_ANGLE_H

#include <Arduino.h>
#include "Sensor.h"

class SteeringAngle : public Sensor {

private:
    int potValue;
    float angle;

public:
    SteeringAngle() : potValue(0), angle(0) {}

    void calculate() override {
        potValue = analogRead(pin);
        angle = map(potValue, 0, 1023, 0, 345);

        tx(&angle, sizeof(angle));
    }
};

#endif

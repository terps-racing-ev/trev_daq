#ifndef STEERING_ANGLE_H
#define STEERING_ANGLE_H

#include <Arduino.h>
#include "Sensor.h"

class SteeringAngle : public Sensor {

private:
    float angle;

public:
    SteeringAngle() : angle(0) {}

    float calculate() override {
        angle = mapFloat(analogRead(pin), 0, 1023, 0, 345);
        tx(&angle, sizeof(angle));
        return angle;
    }
};

#endif

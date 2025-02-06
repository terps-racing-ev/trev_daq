#ifndef STEERING_ANGLE_H
#define STEERING_ANGLE_H

#include <Arduino.h>
#include "Sensor.h"

class SteeringAngle : public Sensor {

private:

public:
    SteeringAngle() {}

    int16_t calculate() override {
        int16_t angle = analogRead(pin) * 3450 / 1023;
        return angle; // Angle in Degrees * 10
    }
};

#endif

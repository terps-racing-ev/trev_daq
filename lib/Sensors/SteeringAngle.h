#ifndef STEERING_ANGLE_H
#define STEERING_ANGLE_H

#include <Arduino.h>
#include "Sensor.h"

class SteeringAngle : public Sensor {

private:

public:
    SteeringAngle() {}

    int16_t calculate() override {
        int16_t angle = map(analogRead(pin), 0, 1023, -1800, 1799);
        return angle; // Angle in Degrees x 10
    }
};

#endif

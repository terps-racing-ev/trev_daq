#ifndef STEERING_ANGLE_H
#define STEERING_ANGLE_H

#include <Arduino.h>
#include "Sensor.h"

class SteeringAngle : public Sensor {

private:

public:
    SteeringAngle() {}

    boolean calculate(void* result) override {
        sa_type* angle_filtered = static_cast<sa_type*>(result);
        if (angle_filtered == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t angle = map(analogRead(pin), 0, 1023, 0, 3599);
        MovingSum_update(&msum, angle);

        *angle_filtered = msum.count < 0 ? msum.sum / msum.count : 0; // Moving average of angle in (Degrees + 180) x 10
        return NO_ERROR;
    }
};

#endif

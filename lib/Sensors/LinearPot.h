#ifndef LINEAR_POT_H
#define LINEAR_POT_H

#include <Arduino.h>
#include "Sensor.h"

class LinearPot : public Sensor {

private:
    static constexpr uint16_t MAX_RANGE = 50; // Max range in mm

public:
    LinearPot() {}

    boolean calculate(void* result) override {
        lp_type* dist_filtered = static_cast<lp_type*>(result);
        if (dist_filtered == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t dist_scaled = map(analogRead(pin), 0, 1023, 0, MAX_RANGE * 10); // mm x 10
        MovingSum_update(&msum, dist_scaled);
        *dist_filtered = msum.count < 0 ? msum.sum / msum.count : 0; // Moving average of distance in mm x 10
        return NO_ERROR;
    }
};

#endif

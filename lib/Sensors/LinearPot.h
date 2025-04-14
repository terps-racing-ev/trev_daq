#ifndef LINEAR_POT_H
#define LINEAR_POT_H

#include <Arduino.h>
#include "AnalogSensor.h"

class LinearPot : public AnalogSensor<lp_type> {

private:
    static constexpr uint16_t MAX_RANGE = 500; // Max range in mm x 10

public:
    LinearPot() {}

    boolean calculate(lp_type* result) override {
        if (result == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t filtered_adc = analog_avg.get_analog_average();

        // Moving average of distance in mm x 10
        lp_type dist_scaled = map(filtered_adc, 0, 1023, 0, MAX_RANGE);
        *result = dist_scaled;
        return NO_ERROR;
    }
};

#endif // LINEAR_POT_H

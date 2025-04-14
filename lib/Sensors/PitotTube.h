#ifndef PITOT_TUBE_H
#define PITOT_TUBE_H

#include <Arduino.h>
#include "AnalogSensor.h"

class PitotTube : public AnalogSensor<pitot_type> {

private:
    static constexpr float AIR_DENSITY = 1.225;

public:
    PitotTube() { }

    boolean calculate(pitot_type* result) override {
        if (result == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t filtered_adc = analog_avg.get_analog_average();
        uint16_t mV = map(filtered_adc, 0, 1023, 0, 5000);

        int16_t Pa = mV - 2740; //voltage to pressure differential conversion
        float vel = sqrt(abs(2.0 * Pa / AIR_DENSITY)) * 2.23694;
        
        // Moving average of Velocity in MPH x 100
        pitot_type vel_scaled = static_cast<pitot_type>(vel * 100);
        *result = vel_scaled;
        return NO_ERROR;
    }
};

#endif // PITOT_TUBE_H

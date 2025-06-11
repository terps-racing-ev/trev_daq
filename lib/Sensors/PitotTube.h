#ifndef PITOT_TUBE_H
#define PITOT_TUBE_H

#include <Arduino.h>
#include "AnalogSensor.h"

class PitotTube : public AnalogSensor<pitot_type> {

private:
    static constexpr float AIR_DENSITY = 1.225;
    static constexpr uint16_t CALIBRATION_READINGS = 100; // 2 seconds, check calculate freq
    uint32_t calibration_sum;
    uint16_t calibration_cnt;
    uint16_t zero_point;

public:
    PitotTube() {
        calibration_sum = 0;
        calibration_cnt = 0;
    }

    boolean calculate(pitot_type* result) override {
        if (result == nullptr) return ERROR;  // Error: result pointer is null

        if (calibration_cnt < CALIBRATION_READINGS) {
            calibration_sum += analogRead(pin);
            calibration_cnt++;
            *result = 0;
            return NO_ERROR;
        } else if (calibration_cnt == CALIBRATION_READINGS) {
            calibration_cnt++;
            zero_point = calibration_sum / CALIBRATION_READINGS;
        }

        uint16_t filtered_adc = analog_avg.get_analog_average();

        int16_t adc_offset = filtered_adc - zero_point;
        float mv_offset = adc_offset * (5000.0/1023.0); // 1 to 1 with Pa theoretically
        float vel = sqrt(abs( 2*(mv_offset) / AIR_DENSITY )) * 2.8;  //find velocity in mph
        
        // Moving average of Velocity in MPH x 100
        pitot_type vel_scaled = static_cast<pitot_type>(vel * 100);
        *result = vel_scaled;
        return NO_ERROR;
    }
};

#endif // PITOT_TUBE_H

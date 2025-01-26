#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>
#include "Sensor.h"

#define ROLLING_RADIUS 900          // hundredths of inches
#define SPOKES 16

#define SCALED_PI 31416             // scaled by 10^4 to avoid floating point arithmetic
#define SCALED_MICROS_IN_HR 3600000 // scaled by 10^-3
#define SCALED_HNTH_IN_IN_MILE 63360 // scaled by 10^-2

// Final answer will be scaled by 10^3 -> 3 decimal places of accuracy
#define MASTER_CONST ((uint64_t(SCALED_MICROS_IN_HR) * 2 * SCALED_PI * ROLLING_RADIUS) / (SPOKES * SCALED_HNTH_IN_IN_MILE)) 

class WheelSpeed : public Sensor {

private:
    volatile uint8_t count;
    volatile uint32_t start_time;
    volatile uint32_t end_time;
    bool reset_flag;

public:
    WheelSpeed() : count(0), start_time(0), end_time(0), reset_flag(false) {}
    
    void intHandler() override {
        end_time = micros();
        if (reset_flag) {
            start_time = end_time;
            reset_flag = false;
        } else {
            count++;
        }
    }
    
    void calculate() override {
        EIMSK &= ~(1 << digitalPinToInterrupt(pin));

        uint32_t diff = end_time - start_time;
        uint32_t mph = diff == 0 ? 0 : (count * MASTER_CONST) / diff;
        if (count > 1) {
            count = 0;
            reset_flag = true;
        }
        EIMSK |= (1 << digitalPinToInterrupt(pin));

        Serial.println(mph);
        //tx(&mph, sizeof(mph));
        
    }
};

#endif

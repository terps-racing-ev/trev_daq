#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>
#include "Sensor.h"

class WheelSpeed : public Sensor {

private:
    static constexpr uint16_t ROLLING_RADIUS = 900;  // Hundredths of inches
    static constexpr uint8_t SPOKES = 16;

    static constexpr uint32_t SCALED_PI = 31416;  // Scaled by 10^4
    static constexpr uint64_t SCALED_MICROS_IN_HR = 3600000;  // Scaled by 10^-3
    static constexpr uint32_t SCALED_HNTH_IN_IN_MILE = 63360;  // Scaled by 10^-2
    static constexpr uint64_t MASTER_CONST = 
        (SCALED_MICROS_IN_HR * 2 * SCALED_PI * ROLLING_RADIUS) 
        / (SPOKES * SCALED_HNTH_IN_IN_MILE);

    // Moving average parameters
    static constexpr uint8_t NUM_READINGS = 5;
    static constexpr uint16_t SPEED_HOLD_TIME = 200;  // Hold last speed for 200ms before resetting

    volatile uint32_t lastPulseTime;
    volatile uint8_t count;

    uint16_t speedReadings[NUM_READINGS];  
    uint8_t readIdx;
    uint32_t speedSum;
    uint16_t lastValidSpeed;

public:
    WheelSpeed() : lastPulseTime(0), count(0), readIdx(0), speedSum(0), lastValidSpeed(0) {
        memset(speedReadings, 0, sizeof(speedReadings));
    }
    
    void intHandler() override {
        uint32_t now = micros();
        lastPulseTime = now;
        count++;
    }
    
    int16_t calculate() override {
        uint32_t now = millis();
        uint32_t diffMicros = micros() - lastPulseTime;
        uint16_t mph_scaled = (diffMicros == 0) ? lastValidSpeed : (count * MASTER_CONST) / diffMicros;

        if (count > 1) {
            count = 0;
        }

        // Moving Average
        speedSum -= speedReadings[readIdx];  
        speedReadings[readIdx] = mph_scaled;
        speedSum += mph_scaled;  

        readIdx = (readIdx + 1) % NUM_READINGS;  

        // If no pulses but within hold time, retain last valid speed
        if (mph_scaled > 0) {
            lastValidSpeed = mph_scaled;
        } else if (now - lastPulseTime >= SPEED_HOLD_TIME) {
            lastValidSpeed = 0;  // Reset when timeout expires
        }

        int16_t avg = speedSum / NUM_READINGS;  
        return avg;
    }
};

#endif

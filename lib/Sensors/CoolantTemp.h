#ifndef COOLANT_TEMP_H
#define COOLANT_TEMP_H

#include <Arduino.h>
#include "Sensor.h"

class CoolantTemp : public Sensor {
private:
    // Stienhart Equation Constants, see matlab file to adjust
    static constexpr float A = -0.0009123445192;
    static constexpr float B = 0.0005229826803;
    static constexpr float C = -0.0000007045640079;

    static constexpr float RESISTOR_VAL = 10030.0;
    static constexpr uint16_t NUM_READINGS = 20;

    uint16_t readings[NUM_READINGS];   // stores velocity values based on numReadings
    uint16_t readIdx;                  // index of the current reading
    uint32_t sum;                      // sum of the last numReadings 

public:
    CoolantTemp() {}

    int16_t calculate() override {
        int16_t mV = map(analogRead(pin), 0, 1023, 0, 5000);
        float resistance = mV * RESISTOR_VAL / (5000 - mV);

        float tempC = pow((A + B * log(resistance) + C * pow(log(resistance), 3)), -1) - 273.15;

        int16_t temp_scaled = static_cast<int16_t>(tempC * 100);
        sum -= readings[readIdx];
        readings[readIdx] = temp_scaled;
        sum += readings[readIdx];
        readIdx = (readIdx + 1) % NUM_READINGS;

        int16_t avg = sum / NUM_READINGS;

        return avg; // Celcius x 100
    }
};

#endif

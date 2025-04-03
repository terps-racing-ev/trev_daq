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

    static constexpr float PULLUP_RESISTOR = 10030.0;
    static constexpr uint8_t NUM_READINGS = 20;

    // Value beyond these will output out of range temps
    // Determined using the Steinhart equation
    static constexpr uint16_t MIN_MV = 534; // -40C
    static constexpr uint16_t MAX_MV = 4853; // 120C

public:
    CoolantTemp() {}

    boolean calculate(void* result) override {
        ct_type* temp_filtered = static_cast<ct_type*>(result);
        if (temp_filtered == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t mV = map(analogRead(pin), 0, 1023, 0, 5000);
        if (mV < MIN_MV || mV > MAX_MV) {
            *temp_filtered = 0;
            return ERROR; // Error: out of range
        }

        float resistance = mV * PULLUP_RESISTOR / (5000 - mV);
        float lnr = log(resistance);
        float tempC = ( 1.0 / (A + B*lnr + C*pow(lnr, 3)) ) - 273.15;

        uint8_t temp_encoded = static_cast<uint16_t>((tempC + 40)); // Celcius + 40
        MovingSum_update(&msum, temp_encoded);
        *temp_filtered = msum.count < 0 ? msum.sum / msum.count : 0; // Moving average of temperature in Celcius + 40
        return NO_ERROR;
    }
};

#endif

#ifndef COOLANT_TEMP_H
#define COOLANT_TEMP_H

#include <Arduino.h>
#include "AnalogSensor.h"

class CoolantTemp : public AnalogSensor<ct_type> {
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

    boolean calculate(ct_type* result) override {
        if (result == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t filtered_adc = analogRead(pin);//analog_avg.get_analog_average();
        uint16_t mV = map(filtered_adc, 0, 1023, 0, 5000);
        if (mV < MIN_MV || mV > MAX_MV) {
            *result = 0;
            return ERROR; // Error: out of range
        }

        float resistance = mV * PULLUP_RESISTOR / (5000 - mV);
        float lnr = log(resistance);
        float tempC = ( 1.0 / (A + B*lnr + C*pow(lnr, 3)) ) - 273.15;
    
        // Moving average of temperature in Celcius
        ct_type temp = static_cast<ct_type>(tempC);
        *result = temp;
        return NO_ERROR;
    }
};

#endif // COOLANT_TEMP_H

#ifndef COOLANT_TEMP_H
#define COOLANT_TEMP_H

#include <Arduino.h>
#include "Sensor.h"

// Stienhart Equation Constants, see matlab file to adjust
// #define A 0.000689287402357408
// #define B 0.000289807635936112
// #define C -0.0000000158833675

#define A -0.9123445192 * pow(10, -3)
#define B 5.229826803 * pow(10, -4)
#define C -7.045640079 * pow(10, -7)

#define RESISTOR_VAL /*10030.0*/ 2200

class CoolantTemp : public Sensor {

private:
    float voltage;
    float resistance;
    float tempC;

public:
    CoolantTemp() : voltage(0), resistance(0), tempC(0) {}

    void calculate() override {
        voltage = analogRead(A2) * (5.0 / 1023.0);

        resistance = voltage * RESISTOR_VAL / (5.00 - voltage);
        Serial.println(resistance);
        tempC = pow((A + B * log(resistance) + C * pow(log(resistance), 3)), -1) - 273.15;
        Serial.println(tempC);

        // TODO: Test that sprintf-ing the float and then tx-ing it works
        char buf[8];
        sprintf(buf, "%f", tempC);

        tx(&buf, sizeof(tempC));
        Serial.printf("Sensor temp: %f °C\n", tempC);
    }
};

#endif

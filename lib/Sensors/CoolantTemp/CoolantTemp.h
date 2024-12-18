#ifndef COOLANTTEMP_H
#define COOLANTTEMP_H

#include <Arduino.h>
#include "CANManager.h"

// Stienhart Equation Constants, see matlab file to adjust
#define A 0.000689287402357408
#define B 0.000289807635936112
#define C -0.0000000158833675

#define RESISTOR_VAL 10030.0

class CoolantTempSensor {

private:
    uint8_t pin;
    float voltage;
    float resistance;
    float tempC;

    CANManager msg;

public:
    CoolantTempSensor() : voltage(0), resistance(0), tempC(0) {}
    void init(uint8_t pin_num, uint16_t id);
    void tx();
};

#endif
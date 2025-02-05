#ifndef BRAKE_PRESSURE_H
#define BRAKE_PRESSURE_H

#include <Arduino.h>
#include "Sensor.h"

class BrakePressure : public Sensor {

private:
    float pressure = 0;

public:
    BrakePressure() : pressure(0) {};
    
    float calculate() override {
        float analogToVoltage = mapFloat(analogRead(pin), 0, 1024, 0, 5);
        pressure = mapFloat(analogToVoltage, 0, 4.5, 0, 3000);
        tx(&pressure, sizeof(pressure));
        return pressure;
    }
};

#endif

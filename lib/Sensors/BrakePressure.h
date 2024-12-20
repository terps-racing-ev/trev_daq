#ifndef BRAKE_PRESSURE_H
#define BRAKE_PRESSURE_H

#include <Arduino.h>
#include "Sensor.h"

class BrakePressure : public Sensor {

private:

public:
    BrakePressure();
    
    void calculate() override {
        double analogToVoltage = map(analogRead(pin), 0, 1024, 0, 5);
        double pressure = map(analogToVoltage, 0, 4.5, 0, 3000);

        tx(&pressure, sizeof(pressure));
    }
};

#endif

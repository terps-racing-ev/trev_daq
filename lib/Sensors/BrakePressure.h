#ifndef BRAKE_PRESSURE_H
#define BRAKE_PRESSURE_H

#include <Arduino.h>
#include "Sensor.h"

class BrakePressure : public Sensor {

private:
    static constexpr uint16_t MIN_MV = 500;
    static constexpr uint16_t MAX_MV = 4500;
    static constexpr uint16_t MIN_PSI = 0;
    static constexpr uint16_t MAX_PSI = 3000;

public:
    BrakePressure() {};
    
    boolean calculate(void* result) override {
        bp_type* psi_filtered = static_cast<bp_type*>(result);
        if (psi_filtered == nullptr) return ERROR;  // Error: result pointer is null

        uint16_t mV = map(analogRead(pin), 0, 1023, 0, 5000);
        if (mV < MIN_MV || mV > MAX_MV) {
            *psi_filtered = 0;
            return ERROR; // Error: out of range
        }

        // Pressure in PSI
        bp_type psi = map(mV, MIN_MV, MAX_MV, MIN_PSI, MAX_PSI);
        MovingSum_update(&msum, psi);
        *psi_filtered = msum.count < 0 ? msum.sum / msum.count : 0; // Moving average of pressure in PSI
        return NO_ERROR;
    }
};

#endif

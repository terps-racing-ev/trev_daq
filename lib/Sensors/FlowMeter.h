#ifndef FLOW_METER_H
#define FLOW_METER_H

#include <Arduino.h>
#include "DigitalSensor.h"

class FlowMeter : public DigitalSensor<flow_type> {


private:
    /*
    Some realistic times between pulses:
    1L/min ~133,333us
    30L/min ~4,500us

    */
    // FLOW_CONST = (1/7.5) * (us in s) * 10x multiplier
    static constexpr uint32_t FLOW_CONST = 1333333;

    static constexpr uint32_t TIMEOUT = 2000000;

    protected:
    // Timeout for pulse duration
    uint32_t getTimeout() const override {
        return TIMEOUT;
    }

public:
    FlowMeter() {}

    boolean calculate(flow_type* result) override {
        if (result == nullptr) return ERROR;  // Error: result pointer is null

        // Moving average of Flow rate in L/min x 10
        flow_type flow_rate = digital_avg.get_digital_average(FLOW_CONST);
        *result = flow_rate;
        return NO_ERROR;
    }
};

#endif // FLOW_METER_H

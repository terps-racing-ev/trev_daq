#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include <Arduino.h>
#include "MovingAvg.h"
#include "SensorTypes.h"

/*
    All Analog Sensors read from ADC and then calculate a value
    This parent class handles the ADC reading and moving average
*/
template<typename SensorType>
class AnalogSensor {
protected:
    uint8_t pin;
    MovingAvg<uint16_t, 128> analog_avg;

public:
    AnalogSensor() : pin(0) {}

    virtual void init(uint8_t pin_num, uint8_t num_readings) {
        pin = pin_num;
        pinMode(pin, INPUT);
    }

    /*
        Updates the moving average with the latest pulse duration
    */
    void update() {
        uint16_t new_sample = analogRead(pin);
        analog_avg.update(new_sample);
    }

    /*  
        Sensor-sepcific calculation logic 
        Returns 1 for success, 0 for error
        Make sure to use the correct sensor type defined in SensorTypes.h
    */
    virtual bool calculate(SensorType* result) = 0;

};

#endif // ANALOG_SENSOR_H

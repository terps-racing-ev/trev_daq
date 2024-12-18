#include "FlowMeter.h"

void FlowMeterSensor::inc() {
    count++;
}

void FlowMeterSensor::init(uint8_t pin_num, void(*intFun)(), uint16_t id) {
    pin = pin_num;
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), intFun, RISING);
    msg.init(id);
}

/* Not yet optimized. Needs to be called every second rn. 
calc was originally an int so this sends uint32 (problem?) */
void FlowMeterSensor::tx() {
    EIMSK &= ~(1 << digitalPinToInterrupt(pin));

    rate = (count * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour
    Serial.println (rate);
    
    msg.tx(&rate, sizeof(rate));

    EIMSK |= (1 << digitalPinToInterrupt(pin));
}
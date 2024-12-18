#include "WheelSpeed.h"

void WheelSpeedSensor::inc() {
    end_time = micros();
    if (reset_flag) {
        start_time = end_time;
        reset_flag = false;
    } else {
        count++;
    }
}

void WheelSpeedSensor::init(uint8_t pin_num, void(*intFun)(), uint16_t id) {
    pin = pin_num;
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), intFun, RISING);
    msg.init(id);
}

/* Sends type uint32 scaled by 1000. Optimized to minimize interrupt close time */
void WheelSpeedSensor::tx() {
    EIMSK &= ~(1 << digitalPinToInterrupt(pin));

    uint32_t diff = end_time - start_time;
    uint32_t mph = diff == 0 ? 0 : (count * MASTER_CONST) / diff;
    if (count > 1) {
        count = 0;
        reset_flag = true;
    }
    Serial.println(mph);
    
    msg.tx(&mph, sizeof(mph));

    EIMSK |= (1 << digitalPinToInterrupt(pin));
}
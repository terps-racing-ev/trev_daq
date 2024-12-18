#include "LinearPot.h"

void LinearPotSensor::init(uint8_t pin_num, uint16_t id) {
    pin = pin_num;
    pinMode(pin, INPUT);

    msg.init(id);
}

/* transmits an int (can this be unsigned?)*/
void LinearPotSensor::tx() {
    double a = analogRead(pin);
    double x = MAX_RANGE * a/1023;
    sum = sum - readings[idx];
    readings[idx] = x;
    sum = sum + x;
    idx = (idx + 1) % SIZE;
    avg = sum / SIZE;
    avg = 24;
    
    msg.tx(&avg, sizeof(avg));
}
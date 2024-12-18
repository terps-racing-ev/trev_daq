#include "SteeringAngle.h"

void SteeringAngleSensor::init(uint8_t pin_num, uint16_t id) {
    pin = pin_num;
    pinMode(pin, INPUT);
    msg.init(id);
}

/* transmits a float */
void SteeringAngleSensor::tx() {
    potValue = analogRead(pin);
    angle = map(potValue, 0, 1023, 0, 345);

    Serial.println(angle);

    msg.tx(&angle, sizeof(angle));
}
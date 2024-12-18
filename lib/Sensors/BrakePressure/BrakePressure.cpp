#include "BrakePressure.h"

void BrakePressureSensor::init(uint8_t pin_num, uint16_t id) {
    pin = pin_num;
    pinMode(pin, INPUT);

    msg.init(id);
}

/* transmits double. should these be class variables? */
void BrakePressureSensor::tx() {
    double analogToVoltage = map(analogRead(pin), 0, 1024, 0, 5);
    double pressure = map(analogToVoltage, 0, 4.5, 0, 3000);

    msg.tx(&pressure, sizeof(pressure));
}
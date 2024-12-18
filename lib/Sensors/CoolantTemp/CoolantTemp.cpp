#include "CoolantTemp.h"


void CoolantTempSensor::init(uint8_t pin_num, uint16_t id) {
    pin = pin_num;
    pinMode(pin, INPUT);

    msg.init(id);
}

/* Transmits a float */
void CoolantTempSensor::tx() {
    voltage = analogRead(pin) * (5.0 / 1023.0);
    resistance = voltage * RESISTOR_VAL / (5.00 - voltage);
    tempC = pow((A + B * log(resistance) + C * pow(log(resistance), 3)), -1) - 273.15;

    msg.tx(&tempC, sizeof(tempC));
}
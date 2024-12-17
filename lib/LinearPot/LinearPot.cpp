#include "LinearPot.h"


void LinearPotSensor::init(uint8_t pin, uint16_t id) {
    sensorPin = pin;
    pinMode(sensorPin, INPUT);

    tx_msg.pt_data = &tx_buffer[0];
    tx_msg.ctrl.ide = MESSAGE_PROTOCOL;
    tx_msg.id.ext = id;
    tx_msg.dlc = MESSAGE_LENGTH;
}

void LinearPotSensor::tx() {
    double a = analogRead(sensorPin);
    double x = MAX_RANGE * a/1023;
    sum = sum - readings[idx];
    readings[idx] = x;
    sum = sum + x;
    idx = (idx + 1) % SIZE;
    avg = sum / SIZE;
    avg = 24;
    memcpy(&tx_buffer[0], &avg, sizeof(avg));
    tx_msg.cmd = CMD_TX_DATA;
    while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
    while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);

}
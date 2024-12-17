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

    tx_msg.pt_data = &tx_buffer[0];
    tx_msg.ctrl.ide = MESSAGE_PROTOCOL;
    tx_msg.id.ext = id;
    tx_msg.dlc = MESSAGE_LENGTH;
}

void WheelSpeedSensor::tx() {
    EIMSK &= ~(1 << digitalPinToInterrupt(pin));

    uint32_t diff = end_time - start_time;
    uint32_t mph = diff == 0 ? 0 : (count * MASTER_CONST) / diff;
    if (count > 1) {
        count = 0;
        reset_flag = true;
    }
    Serial.println(mph);
    memcpy(&tx_buffer[0], &mph, sizeof(mph));
    tx_msg.cmd = CMD_TX_DATA;
    while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
    while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
    EIMSK |= (1 << digitalPinToInterrupt(pin));
}
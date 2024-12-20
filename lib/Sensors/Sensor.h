#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <ASTCanLib.h>

#define MESSAGE_PROTOCOL  0     // 0: CAN 2.0A, 1: CAN 2.0B
#define MESSAGE_LENGTH    8     // Data length

class Sensor {
protected:
    uint8_t pin;
    void (*interruptFun)();     // optional

    st_cmd_t tx_msg;
    uint8_t tx_buffer[MESSAGE_LENGTH];

public:
    Sensor() : interruptFun(nullptr) {
        memset(tx_buffer, 0, sizeof(tx_buffer));
    }

    virtual void init(uint8_t pin_num, uint16_t id, void (*intFun)() = nullptr) {
        pin = pin_num;
        pinMode(pin, INPUT);

        if (intFun != nullptr) {
            interruptFun = intFun;
            attachInterrupt(digitalPinToInterrupt(pin), intFun, RISING);
        }

        tx_msg.pt_data = tx_buffer;
        tx_msg.ctrl.ide = MESSAGE_PROTOCOL;
        tx_msg.id.ext = id;
        tx_msg.dlc = MESSAGE_LENGTH;
    }

    virtual void calculate() = 0;  // Sensor-specific calculation logic

    virtual void tx(const void* data, size_t size) {
        memcpy(tx_buffer, data, size > MESSAGE_LENGTH ? MESSAGE_LENGTH : size);

        tx_msg.cmd = CMD_TX_DATA;
        while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
        while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
    }

    virtual void intHandler() {}  // Optional interrupt handling
};

#endif

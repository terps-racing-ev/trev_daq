#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <Arduino.h>
#include <ASTCanLib.h>


#define MESSAGE_PROTOCOL  0         // 0: CAN 2.0A, 1: CAN 2.0B
#define MESSAGE_LENGTH    8         // Data length

class CANManager {
private:
    st_cmd_t tx_msg;
    uint8_t tx_buffer[8];

public:
    CANManager();
    void init(uint8_t id);
    void tx(const void* data, size_t size);
};

#endif

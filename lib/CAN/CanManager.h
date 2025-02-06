#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <Arduino.h>
#include <ASTCanLib.h>

#define MESSAGE_PROTOCOL  0     // 0: CAN 2.0A, 1: CAN 2.0B
#define MESSAGE_LENGTH    8     // Data length

st_cmd_t tx_msg;
uint8_t tx_buffer[MESSAGE_LENGTH];

void canManagerInit(long baud) {
    canInit(baud);
    tx_msg.pt_data = tx_buffer;
    tx_msg.ctrl.ide = MESSAGE_PROTOCOL;
    tx_msg.dlc = MESSAGE_LENGTH;
    memset(tx_buffer, 0, sizeof(tx_buffer));
}

void tx(uint16_t id, const int16_t* data1, const int16_t* data2 = nullptr,
                const int16_t* data3 = nullptr, const int16_t* data4 = nullptr) 
{
    tx_msg.id.ext = id;
    memset(tx_buffer, 0, sizeof(tx_buffer));
    
    memcpy(tx_buffer, data1, sizeof(int16_t));
    if (data2) memcpy(tx_buffer + sizeof(int16_t), data2, sizeof(int16_t));
    if (data3) memcpy(tx_buffer + 2 * sizeof(int16_t), data3, sizeof(int16_t));
    if (data4) memcpy(tx_buffer + 3 * sizeof(int16_t), data4, sizeof(int16_t));

    tx_msg.cmd = CMD_TX_DATA;
    while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
    while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
}


#endif
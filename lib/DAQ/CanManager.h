#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#define CAN_BAUD_RATE 500000
#define CAN_PROTOCOL 0
#define CAN_MESSAGE_SIZE 8

#include <ASTCanLib.h>

void can_manager_tx(uint16_t can_id, uint8_t* data) {
    st_cmd_t msg;
    msg.id.std = can_id;
    msg.pt_data = data;
    msg.ctrl.ide = CAN_PROTOCOL;
    msg.dlc = CAN_MESSAGE_SIZE;
    msg.cmd = CMD_TX_DATA;

    while (can_cmd(&msg) != CAN_CMD_ACCEPTED);
    while (can_get_status(&msg) == CAN_STATUS_NOT_COMPLETED);
}

#endif // CAN_MANAGER_H

#include "CANManager.h"

CANManager::CANManager() {
    memset(tx_buffer, 0, sizeof(tx_buffer));
}

void CANManager::init(uint8_t id) {
    tx_msg.pt_data = tx_buffer;
    tx_msg.ctrl.ide = MESSAGE_PROTOCOL;
    tx_msg.id.ext = id;
    tx_msg.dlc = MESSAGE_LENGTH;
}

void CANManager::tx(const void* data, size_t size) {
    memcpy(tx_buffer, data, size > 8 ? 8 : size);

    tx_msg.cmd = CMD_TX_DATA;
    while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
    while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
}

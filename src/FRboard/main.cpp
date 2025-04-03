#include "BoardConfig.h"
#include "BrakePressure.h"
#include "PitotTube.h"
#include "SteeringAngle.h"

BrakePressure backBP;
PitotTube pitot;
SteeringAngle steering;

bp_type backBP_val;
pitot_type pitot_val;
sa_type steering_val;

st_cmd_t tx_msg;
uint8_t tx_buffer[CAN_MESSAGE_SIZE];

unsigned long prevTime = 0;
const unsigned long interval = 50;//MILLIS_IN_SEC / FR_BOARD_FREQ;

void setup() {
    canInit(CAN_BAUD_RATE);
    Serial.begin(9600);

    tx_msg.id.std = FR_BOARD_CAN_ID;
    tx_msg.pt_data = tx_buffer;
    tx_msg.ctrl.ide = CAN_PROTOCOL;
    tx_msg.dlc = CAN_MESSAGE_SIZE;

    backBP.init(B_BP_PIN);
    pitot.init(PITOT_PIN);
    steering.init(SA_PIN);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        clearBuffer(tx_buffer);

        if(backBP.calculate(&backBP_val) == NO_ERROR) {
            tx_buffer[0] = backBP_val & 0xFF; // Low byte
            tx_buffer[1] = (backBP_val >> 8) & 0xFF; // High byte
        }
        if(pitot.calculate(&pitot_val) == NO_ERROR) {
            tx_buffer[2] = pitot_val & 0xFF; // Low byte
            tx_buffer[3] = (pitot_val >> 8) & 0xFF; // High byte
        }
        if(steering.calculate(&steering_val) == NO_ERROR) {
            tx_buffer[4] = steering_val & 0xFF; // Low byte
            tx_buffer[5] = (steering_val >> 8) & 0xFF; // High byte
        }

        tx_msg.cmd = CMD_TX_DATA;
        while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
        while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
    }
}

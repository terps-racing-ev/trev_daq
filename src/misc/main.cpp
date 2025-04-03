#include "BoardConfig.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot frLP;
LinearPot flLP;
WheelSpeed frWSP;
WheelSpeed flWSP;

void incfrWSP() { frWSP.intHandler(); }
void incflWSP() { flWSP.intHandler(); }

lp_type frLP_val;
lp_type flLP_val;
wsp_type frWSP_val;
wsp_type flWSP_val;

st_cmd_t tx_msg;
uint8_t tx_buffer[CAN_MESSAGE_SIZE];

unsigned long prevTime = 0;
const unsigned long interval = MILLIS_IN_SEC / FL_BOARD_FREQ;

void setup() {
    canInit(CAN_BAUD_RATE);
    Serial.begin(9600);

    tx_msg.id.std = FL_BOARD_CAN_ID;
    tx_msg.pt_data = tx_buffer;
    tx_msg.ctrl.ide = CAN_PROTOCOL;
    tx_msg.dlc = CAN_MESSAGE_SIZE;

    frLP.init(FR_LP_PIN);
    flLP.init(FL_LP_PIN);
    frWSP.init(FR_WSP_PIN, incfrWSP);
    flWSP.init(FL_WSP_PIN, incflWSP);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        clearBuffer(tx_buffer);

        if(frLP.calculate(&frLP_val) == NO_ERROR) {
            tx_buffer[0] = frLP_val & 0xFF; // Low byte
            tx_buffer[1] = (frLP_val >> 8) & 0xFF; // High byte
        }
        if(flLP.calculate(&flLP_val) == NO_ERROR) {
            tx_buffer[2] = flLP_val & 0xFF; // Low byte
            tx_buffer[3] = (flLP_val >> 8) & 0xFF; // High byte
        }
        if(frWSP.calculate(&frWSP_val) == NO_ERROR) {
            tx_buffer[4] = frWSP_val & 0xFF; // Low byte
            tx_buffer[5] = (frWSP_val >> 8) & 0xFF; // High byte
        }
        if(flWSP.calculate(&flWSP_val) == NO_ERROR) {
            tx_buffer[6] = flWSP_val & 0xFF; // Low byte
            tx_buffer[7] = (flWSP_val >> 8) & 0xFF; // High byte
        }

        tx_msg.cmd = CMD_TX_DATA;
        while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
        while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
    }
}

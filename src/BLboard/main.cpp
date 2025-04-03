#include "BoardConfig.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot brLP;
LinearPot blLP;
WheelSpeed brWSP;
WheelSpeed blWSP;

void incbrWSP() { brWSP.intHandler(); }
void incblWSP() { blWSP.intHandler(); }

lp_type brLP_val;
lp_type blLP_val;
wsp_type brWSP_val;
wsp_type blWSP_val;

st_cmd_t tx_msg;
uint8_t tx_buffer[CAN_MESSAGE_SIZE];

unsigned long prevTime = 0;
const unsigned long interval = MILLIS_IN_SEC / BL_BOARD_FREQ;

void setup() {
    canInit(CAN_BAUD_RATE);
    Serial.begin(9600);

    tx_msg.id.std = BL_BOARD_CAN_ID;
    tx_msg.pt_data = tx_buffer;
    tx_msg.ctrl.ide = CAN_PROTOCOL;
    tx_msg.dlc = CAN_MESSAGE_SIZE;

    brLP.init(BR_LP_PIN);
    blLP.init(BL_LP_PIN);
    brWSP.init(BR_WSP_PIN, incbrWSP);
    blWSP.init(BL_WSP_PIN, incblWSP);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        clearBuffer(tx_buffer);

        if(brLP.calculate(&brLP_val) == NO_ERROR) {
            tx_buffer[0] = brLP_val & 0xFF; // Low byte
            tx_buffer[1] = (brLP_val >> 8) & 0xFF; // High byte
        }
        if(blLP.calculate(&blLP_val) == NO_ERROR) {
            tx_buffer[2] = blLP_val & 0xFF; // Low byte
            tx_buffer[3] = (blLP_val >> 8) & 0xFF; // High byte
        }
        if(brWSP.calculate(&brWSP_val) == NO_ERROR) {
            tx_buffer[4] = brWSP_val & 0xFF; // Low byte
            tx_buffer[5] = (brWSP_val >> 8) & 0xFF; // High byte
        }
        if(blWSP.calculate(&blWSP_val) == NO_ERROR) {
            tx_buffer[6] = blWSP_val & 0xFF; // Low byte
            tx_buffer[7] = (blWSP_val >> 8) & 0xFF; // High byte
        }

        tx_msg.cmd = CMD_TX_DATA;
        while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
        while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
    }
}

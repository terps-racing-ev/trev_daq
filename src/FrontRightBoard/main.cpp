#include <Arduino.h>
#include "ASTCanLib.h"
#include "CanManager.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

//#define DEBUG_SERIAL
#define MILLIS_IN_SEC   1000

/* Front Right Board */
#define FR_BOARD_CAN_ID 0xF1
#define FR_BOARD_UPDT_FREQ 100
#define FR_BOARD_TX_FREQ   50

/* Back Right Board */
#define BR_BOARD_CAN_ID 0xB1

/* Average WSP data */
#define AVERAGE_WSP_CAN_ID 0xFF // fix

#define FR_BOARD_UPDT_PERIOD (MILLIS_IN_SEC / FR_BOARD_UPDT_FREQ)
#define FR_BOARD_TX_PERIOD (MILLIS_IN_SEC / FR_BOARD_TX_FREQ)

#define FR_LP_PIN       A1
#define FL_LP_PIN       A3

#define FR_WSP_PIN      6
#define FL_WSP_PIN      7

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

uint8_t tx_buffer[CAN_MESSAGE_SIZE];
uint8_t rx_buffer[CAN_MESSAGE_SIZE];

unsigned long prev_updt_time = 0;
unsigned long prev_tx_time = 0;

st_cmd_t msg_rx;    // Reading CAN message from BR board
setup_can_rx(&msg_rx, BR_BOARD_CAN_ID, rx_buffer);   // Setting up that shi

bool rx_has_data = false;    // Reading CAN message

void setup() {
    canInit(CAN_BAUD_RATE);

    #ifdef DEBUG_SERIAL
    Serial.begin(9600);
    #endif

    frLP.init(FR_LP_PIN, 10);
    flLP.init(FL_LP_PIN, 10);
    frWSP.init(FR_WSP_PIN, 10, incfrWSP);
    flWSP.init(FL_WSP_PIN, 10, incflWSP);

    can_message_rx(&msg_rx);
}

void loop() {
    unsigned long currTime = millis();

    // If reading, then check if the can message has been received.
    if (!rx_has_data) {
        rx_has_data = (can_get_status(&msg) != CAN_STATUS_NOT_COMPLETED);
    }

    if (currTime - prev_updt_time >= FR_BOARD_UPDT_PERIOD) {
        prev_updt_time = currTime;

        frLP.update();
        flLP.update();
        frWSP.update();
        flWSP.update();
    }
    if (currTime - prev_tx_time >= FR_BOARD_TX_PERIOD) {
        prev_tx_time = currTime;

        clearBuffer(tx_buffer);

        if (rx_has_data) {
            // Do calculations (stored in rx_buffer)
            wsp_type brWSP_val = rx_buffer[4] + (rx_buffer[5] << 8);
            wsp_type blWSP_val = rx_buffer[6] + (rx_buffer[7] << 8);

            wsp_type nums[4] = [brWSP_val, blWSP_val, flWSP_val, frWSP_val];

            for (int i=0; i<4; i++) {
                for (int j=0; j<4-i; j++) {
                    if (nums[i] < nums[j]) {
                        wsp_type temp = nums[i];
                        nums[i] = nums[j];
                        nums[j] = temp;
                    }
                }
            }
            wsp_type average = (wsp_type) ((nums[1] + nums[2])/2.0);

            tx_buffer[0] = average_val & 0xFF; // Low byte
            tx_buffer[1] = (average_val >> 8) & 0xFF; // High byte
            can_manager_tx(AVERAGE_WSP_CAN_ID, tx_buffer);

            can_manager_rx(&msg_rx);

            rx_has_data = false;
        }

        clearBuffer(tx_buffer);

        if (frLP.calculate(&frLP_val) == NO_ERROR) {
            tx_buffer[0] = frLP_val & 0xFF; // Low byte
            tx_buffer[1] = (frLP_val >> 8) & 0xFF; // High byte
        }
        if (flLP.calculate(&flLP_val) == NO_ERROR) {
            tx_buffer[2] = flLP_val & 0xFF; // Low byte
            tx_buffer[3] = (flLP_val >> 8) & 0xFF; // High byte
        }
        if (frWSP.calculate(&frWSP_val) == NO_ERROR) {
            tx_buffer[4] = frWSP_val & 0xFF; // Low byte
            tx_buffer[5] = (frWSP_val >> 8) & 0xFF; // High byte
        }
        if (flWSP.calculate(&flWSP_val) == NO_ERROR) {
            tx_buffer[6] = flWSP_val & 0xFF; // Low byte
            tx_buffer[7] = (flWSP_val >> 8) & 0xFF; // High byte
        }

        can_manager_tx(FR_BOARD_CAN_ID, tx_buffer);
    }
}

#include <Arduino.h>
#include "ASTCanLib.h"
#include "CanManager.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

//#define DEBUG_SERIAL
#define MILLIS_IN_SEC   1000

/* Back Right Board */
#define BR_BOARD_CAN_ID 0xB1
#define BR_BOARD_UPDT_FREQ 100
#define BR_BOARD_TX_FREQ   50

#define BR_BOARD_UPDT_PERIOD (MILLIS_IN_SEC / BR_BOARD_UPDT_FREQ)
#define BR_BOARD_TX_PERIOD (MILLIS_IN_SEC / BR_BOARD_TX_FREQ)

#define BR_LP_PIN       A0
#define BL_LP_PIN       A1

#define BR_WSP_PIN      6
#define BL_WSP_PIN      7

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

uint8_t tx_buffer[CAN_MESSAGE_SIZE];

unsigned long prev_updt_time = 0;
unsigned long prev_tx_time = 0;

void setup() {
    canInit(CAN_BAUD_RATE);

    #ifdef DEBUG_SERIAL
    Serial.begin(9600);
    Serial.println("BR LP, BL LP, BR WSP, BL WSP");
    #endif

    brLP.init(BR_LP_PIN, 10);
    blLP.init(BL_LP_PIN, 10);
    brWSP.init(BR_WSP_PIN, 10, incbrWSP);
    blWSP.init(BL_WSP_PIN, 10, incblWSP);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prev_updt_time >= BR_BOARD_UPDT_PERIOD) {
        prev_updt_time = currTime;

        brLP.update();
        blLP.update();
        brWSP.update();
        blWSP.update();
    }
    if (currTime - prev_tx_time >= BR_BOARD_TX_PERIOD) {
        prev_tx_time = currTime;

        clearBuffer(tx_buffer);

        if (brLP.calculate(&brLP_val) == NO_ERROR) {
            tx_buffer[0] = brLP_val & 0xFF; // Low byte
            tx_buffer[1] = (brLP_val >> 8) & 0xFF; // High byte
        }
        if (blLP.calculate(&blLP_val) == NO_ERROR) {
            tx_buffer[2] = blLP_val & 0xFF; // Low byte
            tx_buffer[3] = (blLP_val >> 8) & 0xFF; // High byte
        }
        if (brWSP.calculate(&brWSP_val) == NO_ERROR) {
            tx_buffer[4] = brWSP_val & 0xFF; // Low byte
            tx_buffer[5] = (brWSP_val >> 8) & 0xFF; // High byte
        }
        if (blWSP.calculate(&blWSP_val) == NO_ERROR) {
            tx_buffer[6] = blWSP_val & 0xFF; // Low byte
            tx_buffer[7] = (blWSP_val >> 8) & 0xFF; // High byte
        }

        can_manager_tx(BR_BOARD_CAN_ID, tx_buffer);

        #ifdef DEBUG_SERIAL
        Serial.print(brLP_val / 10.0f);
        Serial.print(", ");
        Serial.print(blLP_val / 10.0f);
        Serial.print(", ");
        Serial.print(brWSP_val / 100.0f);
        Serial.print(", ");
        Serial.print(blWSP_val / 100.0f);
        Serial.println("");
        #endif
    }
}

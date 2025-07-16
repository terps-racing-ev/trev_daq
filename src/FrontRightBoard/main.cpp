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

#define F_DIST_CAN_ID 0xD1

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

unsigned long prev_updt_time = 0;
unsigned long prev_tx_time = 0;

void setup() {
    canInit(CAN_BAUD_RATE);

    #ifdef DEBUG_SERIAL
    Serial.begin(9600);
    #endif

    frLP.init(FR_LP_PIN, 10);
    flLP.init(FL_LP_PIN, 10);
    frWSP.init(FR_WSP_PIN, 10, incfrWSP);
    flWSP.init(FL_WSP_PIN, 10, incflWSP);
}

void loop() {
    unsigned long currTime = millis();

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

        clearBuffer(tx_buffer);
        cli();
        uint32_t fr_ticks = frWSP.getTicks();
        uint32_t fl_ticks = flWSP.getTicks();     
        sei();

        tx_buffer[0] = fr_ticks & 0xFF;
        tx_buffer[1] = (fr_ticks >> 8) & 0xFF;
        tx_buffer[2] = (fr_ticks >> 16) & 0xFF;
        tx_buffer[3] = (fr_ticks >> 24) & 0xFF;
        tx_buffer[4] = fl_ticks & 0xFF;
        tx_buffer[5] = (fl_ticks >> 8) & 0xFF;
        tx_buffer[6] = (fl_ticks >> 16) & 0xFF;
        tx_buffer[7] = (fl_ticks >> 24) & 0xFF;

        can_manager_tx(F_DIST_CAN_ID, tx_buffer);
    }
}

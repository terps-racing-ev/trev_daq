#include <Arduino.h>
#include "ASTCanLib.h"
#include "CanManager.h"
#include "BrakePressure.h"
#include "PitotTube.h"
#include "SteeringAngle.h"

//#define DEBUG_SERIAL
#define MILLIS_IN_SEC 1000

/* Front Left Board */
#define FL_BOARD_CAN_ID     0xF2
#define FL_BOARD_UPDT_FREQ  100
#define FL_BOARD_TX_FREQ    50

#define FL_BOARD_UPDT_PERIOD (MILLIS_IN_SEC / FL_BOARD_UPDT_FREQ)
#define FL_BOARD_TX_PERIOD (MILLIS_IN_SEC / FL_BOARD_TX_FREQ)

#define B_BP_PIN        A0
#define PITOT_PIN       A1
#define SA_PIN          A2

BrakePressure backBP;
PitotTube pitot;
SteeringAngle steering;

bp_type backBP_val;
pitot_type pitot_val;
uint16_t pitot_raw;
sa_type steering_val;

uint8_t tx_buffer[CAN_MESSAGE_SIZE];

unsigned long prev_updt_time = 0;
unsigned long prev_tx_time = 0;

void setup() {
    canInit(CAN_BAUD_RATE);
    #ifdef DEBUG_SERIAL
    Serial.begin(9600);
    Serial.println("BP, PITOT, SA");
    #endif

    backBP.init(B_BP_PIN, 10);
    pitot.init(PITOT_PIN, 10);
    steering.init(SA_PIN, 10);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prev_updt_time >= FL_BOARD_UPDT_PERIOD) {
        prev_updt_time = currTime;

        backBP.update();
        pitot.update();
        steering.update();

        #ifdef DEBUG_SERIAL
        Serial.print(backBP_val);
        Serial.print(", ");
        Serial.print(pitot_val);
        Serial.print(", ");
        Serial.println(steering_val);
        #endif
    }
    if (currTime - prev_tx_time >= FL_BOARD_UPDT_PERIOD) {
        prev_tx_time = currTime;

        clearBuffer(tx_buffer);

        if(backBP.calculate(&backBP_val) == NO_ERROR) {
            tx_buffer[0] = backBP_val & 0xFF; // Low byte
            tx_buffer[1] = (backBP_val >> 8) & 0xFF; // High byte
        }
        if(pitot.calculate(&pitot_val) == NO_ERROR) {
            tx_buffer[2] = pitot_val & 0xFF; // Low byte
            tx_buffer[3] = (pitot_val >> 8) & 0xFF; // High byte
        }
        if(pitot.raw_reading(&pitot_raw) == NO_ERROR) {
            tx_buffer[4] = pitot_raw & 0xFF; // Low byte
            tx_buffer[5] = (pitot_raw >> 8) & 0xFF; // High byte
        }
        if(steering.calculate(&steering_val) == NO_ERROR) {
            tx_buffer[6] = steering_val & 0xFF; // Low byte
            tx_buffer[7] = (steering_val >> 8) & 0xFF; // High byte
        }

        can_manager_tx(FL_BOARD_CAN_ID, tx_buffer);
    }
}

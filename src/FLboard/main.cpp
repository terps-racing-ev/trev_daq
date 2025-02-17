#include "Config.h"
#include "CanManager.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot frLP;
LinearPot flLP;
WheelSpeed frWSP;
WheelSpeed flWSP;

void incfrWSP() { frWSP.intHandler(); }
void incflWSP() { flWSP.intHandler(); }

unsigned long prevTime = 0;
const unsigned long interval = MILLIS_IN_SEC / FL_BOARD_FREQ;

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    frLP.init(FR_LP_PIN);
    flLP.init(FL_LP_PIN);
    frWSP.init(FR_WSP_PIN, incfrWSP);
    flWSP.init(FL_WSP_PIN, incflWSP);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        int16_t frLP_val = frLP.calculate();
        int16_t flLP_val = flLP.calculate();
        tx(B_LP_CAN_ID, &frLP_val, &flLP_val);
        
        int16_t frWSP_val = frWSP.calculate();
        int16_t flWSP_val = flWSP.calculate();
        tx(B_WSP_CAN_ID, &frWSP_val, &flWSP_val);
    }
}

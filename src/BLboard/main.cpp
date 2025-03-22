#include "Config.h"
#include "CanManager.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot brLP;
LinearPot blLP;
WheelSpeed brWSP;
WheelSpeed blWSP;

void incbrWSP() { brWSP.intHandler(); }
void incblWSP() { blWSP.intHandler(); }

unsigned long prevTime = 0;
const unsigned long interval = MILLIS_IN_SEC / BL_BOARD_FREQ;

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    brLP.init(BR_LP_PIN);
    blLP.init(BL_LP_PIN);
    brWSP.init(BR_WSP_PIN, incbrWSP);
    blWSP.init(BL_WSP_PIN, incblWSP);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        int16_t brLP_val = brLP.calculate();
        int16_t blLP_val = blLP.calculate();
        Serial.println(brLP_val);
        tx(B_LP_CAN_ID, &brLP_val, &blLP_val);

        int16_t brWSP_val = brWSP.calculate();
        int16_t blWSP_val = blWSP.calculate();
        tx(B_WSP_CAN_ID, &brWSP_val, &blWSP_val);
    }
}

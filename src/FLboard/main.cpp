#include <Arduino.h>
#include "CanManager.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot frLP;
LinearPot flLP;
WheelSpeed frWSP;
WheelSpeed flWSP;

void incfrWSP() { frWSP.intHandler(); }
void incflWSP() { flWSP.intHandler(); }

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    frLP.init(A1);
    flLP.init(A3);
    frWSP.init(6, incfrWSP);
    flWSP.init(7, incflWSP);
}

void loop() {
    int16_t frLP_val = frLP.calculate();
    int16_t flLP_val = flLP.calculate();
    tx(540, &frLP_val, &flLP_val);
    int16_t frWSP_val = frWSP.calculate();
    int16_t flWSP_val = flWSP.calculate();
    tx(440, &frWSP_val, &flWSP_val);
    delay(20);
}
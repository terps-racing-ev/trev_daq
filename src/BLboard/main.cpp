#include <Arduino.h>
#include "CanManager.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot brLP;
LinearPot blLP;
WheelSpeed brWSP;
WheelSpeed blWSP;

void incbrWSP() { brWSP.intHandler(); }
void incblWSP() { blWSP.intHandler(); }

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    brLP.init(A0);
    blLP.init(A1);
    brWSP.init(6, incbrWSP);
    blWSP.init(7, incblWSP);
}

void loop() {
    int16_t brLP_val = brLP.calculate();
    int16_t blLP_val = blLP.calculate();
    tx(530, &brLP_val, &blLP_val);
    int16_t brWSP_val = brWSP.calculate();
    int16_t blWSP_val = blWSP.calculate();
    tx(430, &brWSP_val, &blWSP_val);
    delay(20);
}
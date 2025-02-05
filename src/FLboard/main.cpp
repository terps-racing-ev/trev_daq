#include <Arduino.h>
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot frLP;
LinearPot flLP;
WheelSpeed frWSP;
WheelSpeed flWSP;


void incfrWSP() { frWSP.intHandler(); }
void incflWSP() { flWSP.intHandler(); }

void setup() {
    canInit(500000);
    Serial.begin(9600);

    frLP.init(A1, 541);
    flLP.init(A3, 542);
    frWSP.init(6, 441, incflWSP);
    flWSP.init(7, 442, incfrWSP);
}

void loop() {
    frLP.calculate();
    flLP.calculate();
    frWSP.calculate();
    flWSP.calculate();
    delay(20);
}
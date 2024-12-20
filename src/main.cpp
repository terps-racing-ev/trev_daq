#include <Arduino.h>
#include "WheelSpeed.h"
#include "LinearPot.h"

WheelSpeed flWSP;
WheelSpeed frWSP;
LinearPot flLP;
LinearPot frLP;

void incflWSP() { flWSP.intHandler(); }
void incfrWSP() { frWSP.intHandler(); }

void setup() {
    canInit(500000);
    Serial.begin(9600);

    flWSP.init(6, 304, incflWSP);
    frWSP.init(7, 301, incfrWSP);
    flLP.init(A0, 404);
    frLP.init(A1, 401);
}

void loop() {
    flWSP.calculate();
    frWSP.calculate();
    flLP.calculate();
    frLP.calculate();
    delay(1000);
}
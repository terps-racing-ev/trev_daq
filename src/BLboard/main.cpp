#include <Arduino.h>
#include "WheelSpeed.h"
#include "LinearPot.h"

WheelSpeed blWSP;
WheelSpeed brWSP;
LinearPot blLP;
LinearPot brLP;

void incflWSP() { blWSP.intHandler(); }
void incfrWSP() { brWSP.intHandler(); }

void setup() {
    canInit(500000);
    Serial.begin(9600);

    blWSP.init(6, 304, incflWSP);
    brWSP.init(7, 301, incfrWSP);
    blLP.init(A0, 404);
    brLP.init(A1, 401);
}

void loop() {
    blWSP.calculate();
    brWSP.calculate();
    blLP.calculate();
    brLP.calculate();
    delay(20);
}
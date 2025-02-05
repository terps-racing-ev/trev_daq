#include <Arduino.h>
#include "LinearPot.h"
#include "WheelSpeed.h"


LinearPot brLP;
LinearPot blLP;
WheelSpeed brWSP;
WheelSpeed blWSP;


void incbrWSP() { brWSP.intHandler(); }
void incblWSP() { blWSP.intHandler(); }

void setup() {
    canInit(500000);
    Serial.begin(9600);

    brLP.init(A0, 531);
    blLP.init(A1, 532);
    brWSP.init(6, 431, incbrWSP);
    blWSP.init(7, 432, incbrWSP);
}

void loop() {
    brLP.calculate();
    blLP.calculate();
    brWSP.calculate();
    blWSP.calculate();
    
    delay(20);
}
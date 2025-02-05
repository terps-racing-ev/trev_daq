#include <Arduino.h>
#include "BrakePressure.h"
#include "WheelSpeed.h"


BrakePressure brBP;
BrakePressure blBP;
WheelSpeed brWSP;
WheelSpeed blWSP;


void incbrWSP() { brWSP.intHandler(); }
void incblWSP() { blWSP.intHandler(); }

void setup() {
    canInit(500000);
    Serial.begin(9600);

    brBP.init(A0, 631);
    blBP.init(A1, 632);
    brWSP.init(6, 431, incbrWSP);
    blWSP.init(7, 432, incbrWSP);
}

void loop() {
    brBP.calculate();
    blBP.calculate();
    brWSP.calculate();
    blWSP.calculate();
    
    delay(20);
}
#include <Arduino.h>
#include "CanManager.h"
#include "BrakePressure.h"
#include "WheelSpeed.h"

BrakePressure brBP;
BrakePressure blBP;
WheelSpeed brWSP;
WheelSpeed blWSP;

void incbrWSP() { brWSP.intHandler(); }
void incblWSP() { blWSP.intHandler(); }

void setup() {
    canManagerInit(500000);
    //Serial.begin(9600);

    brBP.init(A0);
    blBP.init(A1);
    brWSP.init(6, incbrWSP);
    blWSP.init(7, incblWSP);
}

void loop() {
    int16_t brBP_val = brBP.calculate();
    int16_t blBP_val = blBP.calculate();
    tx(630, &brBP_val, &blBP_val);
    int16_t brWSP_val = brWSP.calculate();
    int16_t blWSP_val = blWSP.calculate();
    tx(430, &brWSP_val, &blWSP_val);
    delay(20);
}
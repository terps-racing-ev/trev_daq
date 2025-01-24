#include <Arduino.h>
#include "WheelSpeed.h"
#include "LinearPot.h"
#include "CoolantTemp.h"

WheelSpeed flWSP;
WheelSpeed frWSP;
LinearPot flLP;
LinearPot frLP;

CoolantTemp col;

void incflWSP() { flWSP.intHandler(); }
void incfrWSP() { frWSP.intHandler(); }

#define A 0.000689287402357408
#define B 0.000289807635936112
#define C -0.0000000158833675

#define RESISTOR_VAL /*10030.0*/ 2200

float voltage;
float resistance;
float tempC;

void setup() {
    canInit(500000);
    Serial.begin(9600);

    flWSP.init(6, 304, incflWSP);
    frWSP.init(7, 301, incfrWSP);
    flLP.init(A0, 404);
    frLP.init(A1, 401);
    col.init(A3, 420);
}


void loop() {
    // flWSP.calculate();
    // frWSP.calculate();
    // flLP.calculate();
    // frLP.calculate();
    col.calculate();
    delay(1000);
}


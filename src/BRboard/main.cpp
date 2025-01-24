#include <Arduino.h>
#include "CoolantTemp.h"
#include "FlowMeter.h"

CoolantTemp c1;
CoolantTemp c2;
CoolantTemp c3;
CoolantTemp c4;

FlowMeter flowmeter;

void incflow() { flowmeter.intHandler(); }

void setup() {
    canInit(500000);
    Serial.begin(9600);

    c1.init(A0, 100);
    c2.init(A1, 101);
    c3.init(A2, 102);
    c4.init(A3, 103);

    flowmeter.init(6, 200, incflow);
}

void loop() {
    c1.calculate();
    c2.calculate();
    c3.calculate();
    c4.calculate();
    flowmeter.calculate();
    delay(50);
}
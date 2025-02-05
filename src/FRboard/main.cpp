#include <Arduino.h>
#include "LinearPot.h"
#include "PitotTube.h"
#include "SteeringAngle.h"

LinearPot frLP;
LinearPot flLP;
PitotTube pitot;
SteeringAngle steering;

void setup() {
    canInit(500000);
    Serial.begin(9600);

    frLP.init(A0, 511);
    flLP.init(A1, 512);
    pitot.init(A2, 710);
    steering.init(A3, 810);
}

void loop() {
    frLP.calculate();
    flLP.calculate();
    pitot.calculate();
    steering.calculate();
    delay(20);
}
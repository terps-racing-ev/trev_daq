#include <Arduino.h>
#include "BrakePressure.h"
#include "PitotTube.h"
#include "SteeringAngle.h"

BrakePressure rBP;
BrakePressure lBP;
PitotTube pitot;
SteeringAngle steering;

void setup() {
    canInit(500000);
    Serial.begin(9600);

    rBP.init(A0, 611);
    lBP.init(A1, 612);
    pitot.init(A2, 710);
    steering.init(A3, 810);
}

void loop() {
    rBP.calculate();
    lBP.calculate();
    pitot.calculate();
    steering.calculate();
    delay(20);
}
#include <Arduino.h>
#include "CanManager.h"
#include "LinearPot.h"
#include "PitotTube.h"
#include "SteeringAngle.h"

LinearPot frLP;
LinearPot flLP;
PitotTube pitot;
SteeringAngle steering;

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    frLP.init(A0);
    flLP.init(A1);
    pitot.init(A2);
    steering.init(A3);
}

void loop() {
    int16_t frLP_val = frLP.calculate();
    int16_t flLP_val = flLP.calculate();
    tx(510, &frLP_val, &flLP_val);
    int16_t pitot_val = pitot.calculate();
    tx(710, &pitot_val);
    int16_t steering_val = steering.calculate();
    tx(810, &steering_val);
    delay(20);
}
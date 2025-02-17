#include <Arduino.h>
#include "CanManager.h"
#include "BrakePressure.h"
#include "PitotTube.h"
#include "SteeringAngle.h"

BrakePressure backBP;
PitotTube pitot;
SteeringAngle steering;

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    backBP.init(A0);
    pitot.init(A1);
    steering.init(A2);
}

void loop() {
    int16_t backBP_val = backBP.calculate();
    tx(610, &backBP_val, &backBP_val);
    int16_t pitot_val = pitot.calculate();
    tx(710, &pitot_val);
    int16_t steering_val = steering.calculate();
    tx(810, &steering_val);
    delay(20);
}
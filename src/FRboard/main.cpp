#include "Config.h"
#include "CanManager.h"
#include "BrakePressure.h"
#include "PitotTube.h"
#include "SteeringAngle.h"

BrakePressure backBP;
PitotTube pitot;
SteeringAngle steering;

unsigned long prevTime = 0;
const unsigned long interval = MILLIS_IN_SEC / FR_BOARD_FREQ;

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    backBP.init(B_BP_PIN);
    pitot.init(PITOT_PIN);
    steering.init(SA_PIN);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        int16_t backBP_val = backBP.calculate();
        tx(B_BP_CAN_ID, &backBP_val, &backBP_val);
        int16_t pitot_val = pitot.calculate();
        tx(PITOT_CAN_ID, &pitot_val);
        int16_t steering_val = steering.calculate();
        tx(SA_CAN_ID, &steering_val);
    }
}

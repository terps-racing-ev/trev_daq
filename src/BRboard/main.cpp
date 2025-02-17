#include "Config.h"
#include "CanManager.h"
#include "CoolantTemp.h"
#include "FlowMeter.h"

const int16_t tempPoints[5] = {25, 35, 45, 60, 75};
const uint8_t fanSpeeds[5] = {0, 64, 128, 192, 255};

CoolantTemp temps[4];
FlowMeter flowmeter;

int16_t temp_vals[4];
int16_t flow_val;

unsigned long prevTime = 0;
const unsigned long interval = MILLIS_IN_SEC / BR_BOARD_FREQ;

void incflow() { flowmeter.intHandler(); }

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    for (int i = 0; i < 4; i++){
        temps[i].init(BASE_CT_PIN + i); // A0-A4
    }

    flowmeter.init(FLOW_PIN, incflow);
    analogWrite(PWM_PIN, 200); // PUMP NEEDS TO BE INITIALIZED
    delay(500);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        flow_val = flowmeter.calculate();

        int16_t maxTemp = 0;
        for (uint8_t i = 0; i < 4; i++) {
            temp_vals[i] = temps[i].calculate();
        }
        
        uint8_t pwm_val = getFanSpeed(maxTemp);
        analogWrite(PWM_PIN, pwm_val);
    }
}


uint8_t getFanSpeed(int16_t currTemp) {
    int16_t speed = 0;

    // Check which interval the temperature falls into
    for (uint8_t i = 0; i < 4; i++) {
      if (currTemp >= tempPoints[i] &&
          currTemp <= tempPoints[i + 1]) {
        // Interpolate fan speed within this temperature range
        float ratio = (float)(currTemp - tempPoints[i]) /
                      (tempPoints[i + 1] - tempPoints[i]);
        speed = fanSpeeds[i] + ratio * (fanSpeeds[i + 1] - fanSpeeds[i]);
        break;
      }
    }

    // If above max temperature point, set fan to max speed
    if (currTemp >= tempPoints[4]) {
      speed = fanSpeeds[4];
    }

    return constrain(speed, 0, 255);  // Clamp to 0-255
}

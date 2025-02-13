#include <Arduino.h>
#include "CanManager.h"
#include "CoolantTemp.h"
#include "FlowMeter.h"

CoolantTemp temps[4];
FlowMeter flowmeter;

int16_t temp_vals[4];
int16_t flow_val;


const int16_t tempPoints[5] = {25, 35, 45, 60, 75};
const uint8_t fanSpeeds[5] = {0, 64, 128, 192, 255};


void incflow() { flowmeter.intHandler(); }

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    for (int i = 0; i < 4; i++){
        temps[i].init(A0 + i); // A0-A4
    }

    flowmeter.init(A6, incflow);
}

// TODO
void loop() {
    
    for (uint8_t i = 0; i < 4; i++) {
        temp_vals[i] = temps[i].calculate();
    }
    flow_val = flowmeter.calculate();
    delay(500); // 2Hz
}


int16_t getFanSpeed(int16_t currTemp) {
    int speed = 0;

    // Check which interval the temperature falls into
    for (int i = 0; i < 4; i++) {
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
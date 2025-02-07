#include <Arduino.h>
#include "CanManager.h"
#include "CoolantTemp.h"
#include "FlowMeter.h"

CoolantTemp c1;
CoolantTemp c2;
CoolantTemp c3;
CoolantTemp c4;

FlowMeter flowmeter;

void incflow() { flowmeter.intHandler(); }

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    c1.init(A0);
    c2.init(A1);
    c3.init(A2);
    c4.init(A3);

    flowmeter.init(6, incflow);
}

void loop() {
    //c1.calculate();
    //c2.calculate();
    //c3.calculate();
    //c4.calculate();
    flowmeter.calculate();
    delay(500);
}

/*

  const int tempPoints[5];
  const int fanSpeeds[5];

  tempPoints({25, 35, 45, 60, 75}),  // Temp points
        fanSpeeds({0, 64, 128, 192, 255})  // Corresponding fan speeds


int getFanSpeed(int currentTemperature) {
    int speed = 0;

    // Check which interval the temperature falls into
    for (int i = 0; i < 4; i++) {
      if (currentTemperature >= tempPoints[i] &&
          currentTemperature <= tempPoints[i + 1]) {
        // Interpolate fan speed within this temperature range
        float ratio = (float)(currentTemperature - tempPoints[i]) /
                      (tempPoints[i + 1] - tempPoints[i]);
        speed = fanSpeeds[i] + ratio * (fanSpeeds[i + 1] - fanSpeeds[i]);
        break;
      }
    }

    // If above max temperature point, set fan to max speed
    if (currentTemperature >= tempPoints[4]) {
      speed = fanSpeeds[4];
    }

    return constrain(speed, 0, 255);  // Clamp to 0-255
  }
  */
#ifndef COOLANT_TEMP_H
#define COOLANT_TEMP_H

#include <Arduino.h>

#include "Sensor.h"

// Stienhart Equation Constants, see matlab file to adjust
// #define A 0.000689287402357408
// #define B 0.000289807635936112
// #define C -0.0000000158833675

#define A -0.9123445192 * pow(10, -3)
#define B 5.229826803 * pow(10, -4)
#define C -7.045640079 * pow(10, -7)

#define RESISTOR_VAL /*10030.0*/ 2200

class CoolantTemp : public Sensor {
 private:
  float voltage;
  float resistance;
  float tempC;

  const int tempPoints[5];
  const int fanSpeeds[5];

 public:
  CoolantTemp()
      : voltage(0),
        resistance(0),
        tempC(0),
        tempPoints({25, 35, 45, 60, 75}),  // Temp points
        fanSpeeds({0, 64, 128, 192, 255})  // Corresponding fan speeds
  {}

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

  float getTemp() {
    voltage = analogRead(A2) * (5.0 / 1023.0);

    resistance = voltage * RESISTOR_VAL / (5.00 - voltage);
    Serial.println(resistance);
    tempC = pow((A + B * log(resistance) + C * pow(log(resistance), 3)), -1) -
            273.15;
    Serial.println(tempC);

    // TODO: Test that sprintf-ing the float and then tx-ing it works
    return tempC;
  }

  void calculate() override {
    char buf[8];
    *buf = getTemp();
    sprintf(buf, "%f", tempC);

    tx(&buf, sizeof(tempC));
    Serial.printf("Sensor temp: %f °C\n", tempC);
  }
};

#endif
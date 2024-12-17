#include "BrakePressure.h"

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  double analogToVoltage = map(analogRead(A0), 0, 1024, 0, 5);
  Serial.println(map(analogToVoltage, 0, 4.5, 0, 3000));
}

#include <Arduino.h>
#include "WheelSpeed.h"
#include "LinearPot.h"

WheelSpeedSensor flWSP;
WheelSpeedSensor frWSP;
LinearPotSensor flLP;
LinearPotSensor frLP;

void incflWSP() { flWSP.inc(); }
void incfrWSP() { frWSP.inc(); }

void setup() {
  canInit(500000);
  Serial.begin(9600);

  pinMode(6, INPUT);
  flWSP.init(6, incflWSP, 304);
  //frWSP.init(7, incfrWSP, 301);
  //flLP.init(A0, 404);
  //frLP.init(A1, 401);
}

void loop() {
  flWSP.tx();
  //frWSP.tx();
  //flLP.tx();
  //frLP.tx();
  //Serial.println(digitalRead(6));
  delay(1000);
}
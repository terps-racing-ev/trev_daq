#include <Arduino.h>


void setup() {
    //canInit(500000);
    Serial.begin(9600);

    pinMode(A0, INPUT);
}

void loop() {
    Serial.println(analogRead(A0));
    delay(100);
}
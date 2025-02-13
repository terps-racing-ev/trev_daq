#include <Arduino.h>
#include "CanManager.h"
#include "LinearPot.h"
#include "WheelSpeed.h"

LinearPot one;
LinearPot two;
WheelSpeed three;
WheelSpeed four;

void incThree() { three.intHandler(); }
void incFour() { four.intHandler(); }

void setup() {
    canManagerInit(500000);
    Serial.begin(9600);

    one.init(A0);
    two.init(A1);
    three.init(6, incThree);
    four.init(7, incFour);
}

void loop() {
        //int16_t one_val = one.calculate();
        //int16_t two_val = two.calculate();
        //int16_t three_val = three.calculate();
        int16_t four_val = four.calculate();
        
        //Serial.println(one_val);
        //Serial.println(two_val);
        //Serial.println(three_val);
        Serial.println(four_val);

        delay(500);
}
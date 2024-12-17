#include "SteeringAngle.h"

const int potPin = A0;  // Define the analog input pin for the potentiometer
int potValue = 0;        // Variable to store the potentiometer value
float angle = 0;         // Variable to store the angle in degrees
void setup() {
  Serial.begin(9600);  // Start serial communication
}
void loop() {
  potValue = analogRead(potPin);  // Read the potentiometer value (0-1023)
  // Map the potentiometer value to a range of 0-345 degrees
  angle = map(potValue, 0, 1023, 0, 345);
  Serial.println(angle);  // Print the angle value
  delay(100);  // Small delay before the next reading
}
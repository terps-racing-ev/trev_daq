#include <Arduino.h>
#include "ASTCanLib.h"
#include "CanManager.h"
#include "CoolantTemp.h"
#include "FlowMeter.h"

//#define DEBUG_SERIAL
#define MILLIS_IN_SEC   1000

/* Cooling Board */
#define COOLING_CAN_ID          0xCB
#define COOLING_BOARD_UPDT_FREQ 100
#define COOLING_BOARD_TX_FREQ   2

#define COOLING_BOARD_UPDT_PERIOD (MILLIS_IN_SEC / COOLING_BOARD_UPDT_FREQ)
#define COOLING_BOARD_TX_PERIOD (MILLIS_IN_SEC / COOLING_BOARD_TX_FREQ)

#define NUM_CT_SENSORS  4
#define BASE_CT_PIN     A0
#define FLOW_PIN        6

#define PWM_PIN         5

#define MAP_RES 7 // Number of temp to fan speed mappings

const ct_type tempPoints[MAP_RES] = {17,  22,  27,  30,  35,  45,  50};
const uint8_t fanSpeeds[MAP_RES] =  { 64, 64, 100, 128, 192, 255, 255};

CoolantTemp temps[NUM_CT_SENSORS];
FlowMeter flowmeter;

void incflow() { flowmeter.intHandler(); }

ct_type temp_vals[NUM_CT_SENSORS];
flow_type flow_val;

uint8_t tx_buffer[CAN_MESSAGE_SIZE];

unsigned long prev_updt_time = 0;
unsigned long prev_tx_time = 0;

uint8_t getFanSpeed(int8_t currTemp);

void setup() {
    canInit(CAN_BAUD_RATE);

    #ifdef DEBUG_SERIAL
    Serial.begin(9600);
    Serial.println("PWM, FLOW, CT1, CT2, CT3, CT4");
    #endif

    for (int i = 0; i < 4; i++){
        temps[i].init(BASE_CT_PIN + i, 100); // A0-A4
    }
    flowmeter.init(FLOW_PIN, 100, incflow);
    /* Pump Initialization */
    analogWrite(PWM_PIN, 200);
    delay(500);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prev_updt_time >= COOLING_BOARD_UPDT_PERIOD) {
        prev_updt_time = currTime;

        ct_type maxTemp = -40;

        for (uint8_t i = 0; i < NUM_CT_SENSORS; i++) {
            if (temps[i].calculate(&temp_vals[i]) == NO_ERROR) {
                if (temp_vals[i] > maxTemp) {
                    maxTemp = static_cast<uint16_t>(temp_vals[i]) - 40;
                }
            } else {
                maxTemp = 255; // Assume max temp if a sensor errors
            }
        }
        flowmeter.update();

        uint8_t pwm_val = getFanSpeed(maxTemp - 40);
        analogWrite(PWM_PIN, pwm_val);
        
        #ifdef DEBUG_SERIAL
        Serial.print(pwm_val);
        Serial.print(", ");
        Serial.print(flow_val / 10.0f);
        for (uint8_t i = 0; i < NUM_CT_SENSORS; i++) {
            Serial.print(", ");
            Serial.print(temp_vals[i] - 40);
        }
        Serial.println("");
        #endif
    }

    if (currTime - prev_tx_time >= COOLING_BOARD_TX_PERIOD) {
        prev_tx_time = currTime;

        for (uint8_t i = 0; i < NUM_CT_SENSORS; i++) {
            if (temps[i].calculate(&temp_vals[i]) == NO_ERROR) {
                tx_buffer[i] = temp_vals[i];
            }
        }
        if (flowmeter.calculate(&flow_val) == NO_ERROR) {
            tx_buffer[6] = flow_val & 0xFF; // Low byte
            tx_buffer[7] = (flow_val >> 8) & 0xFF; // High byte
        }
    }
}


uint8_t getFanSpeed(ct_type currTemp) {
    uint8_t speed = 0;

    // Check which interval the temperature falls into
    for (uint8_t i = 0; i < MAP_RES - 1; i++) {
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
    if (currTemp >= tempPoints[MAP_RES - 1]) {
      speed = fanSpeeds[MAP_RES - 1];
    }

    return constrain(speed, 0, 255);  // Clamp to 0-255
}

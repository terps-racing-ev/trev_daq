#include "BoardConfig.h"
#include "CoolantTemp.h"
#include "FlowMeter.h"

#define MAP_RES         7 // Number of temp to fan speed mappings

const ct_type tempPoints[MAP_RES] = {10,  22,  27,  30,  35,  45,  50};
const uint8_t fanSpeeds[MAP_RES] =  {0,   0,   64,  128, 192, 255, 255};

CoolantTemp temps[NUM_CT_SENSORS];
FlowMeter flowmeter;

ct_type temp_vals[NUM_CT_SENSORS];
flow_type flow_val;

st_cmd_t tx_msg;
uint8_t tx_buffer[CAN_MESSAGE_SIZE];

unsigned long prevTime = 0;
const unsigned long interval = MILLIS_IN_SEC / BR_BOARD_FREQ;

void incflow() { flowmeter.intHandler(); }

void setup() {
    canInit(CAN_BAUD_RATE);
    Serial.begin(9600);

    tx_msg.id.std = COOLING_CAN_ID;
    tx_msg.pt_data = tx_buffer;
    tx_msg.ctrl.ide = CAN_PROTOCOL;
    tx_msg.dlc = CAN_MESSAGE_SIZE;
    tx_msg.cmd = CMD_TX_DATA;

    for (int i = 0; i < 4; i++){
        temps[i].init(BASE_CT_PIN + i); // A0-A4
    }
    flowmeter.init(FLOW_PIN, incflow);
    /* Pump Initialization */
    analogWrite(PWM_PIN, 200);
    delay(500);
}

void loop() {
    unsigned long currTime = millis();

    if (currTime - prevTime >= interval) {
        prevTime = currTime;

        clearBuffer(tx_buffer);

        ct_type maxTemp = 0;

        for (uint8_t i = 0; i < NUM_CT_SENSORS; i++) {
            if (temps[i].calculate(&temp_vals[i]) == NO_ERROR) {
                tx_buffer[i] = temp_vals[i];
                if (temp_vals[i] > maxTemp) {
                    maxTemp = temp_vals[i];
                }
            } else {
                maxTemp = 255; // Assume max temp if a sensor errors
            }
        }
        if (flowmeter.calculate(&flow_val) == NO_ERROR) {
            tx_buffer[6] = flow_val & 0xFF; // Low byte
            tx_buffer[7] = (flow_val >> 8) & 0xFF; // High byte
        }

        uint8_t pwm_val = getFanSpeed(maxTemp);
        analogWrite(PWM_PIN, pwm_val);

        tx_msg.cmd = CMD_TX_DATA;
        while (can_cmd(&tx_msg) != CAN_CMD_ACCEPTED);
        while (can_get_status(&tx_msg) == CAN_STATUS_NOT_COMPLETED);
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

#ifndef LINEARPOT_H
#define LINEARPOT_H

#include <Arduino.h>
#include <ASTCanLib.h>

#define MESSAGE_PROTOCOL  0         // 0: CAN 2.0A, 1: CAN 2.0B
#define MESSAGE_LENGTH    8         // Data length

#define SIZE 5
#define MAX_RANGE 50

class LinearPotSensor {

private:

uint8_t sensorPin;

int idx;
int sum;
int readings[SIZE];
int avg;

st_cmd_t tx_msg;
uint8_t tx_buffer[MESSAGE_LENGTH];

public:
    LinearPotSensor() : idx(0), sum(0), avg(0) {
        memset(readings, 0, sizeof(readings));
    }
    void init(uint8_t pin, uint16_t id);
    void tx();

};

#endif

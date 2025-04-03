#ifndef MOVING_SUM_H
#define MOVING_SUM_H

#include <Arduino.h>

#define MAX_READINGS 32

/* Only keeps track of a moving sum since 
different sensors use different calculations 
to get their respective averages */

typedef struct {
    uint32_t readings[MAX_READINGS];
    uint32_t sum; // 32 is ok here because 1,000,000 should be the largest input for flow and wspd
    uint8_t index;
    uint8_t count; // Used on startup to prevent 0's from being averaged
    uint8_t num_readings;
} MovingSum;

void MovingSum_init(MovingSum* msum, uint8_t num_readings) {
    if (num_readings > MAX_READINGS) num_readings = MAX_READINGS;

    for (int i = 0; i < MAX_READINGS; i++) {
        msum->readings[i] = 0;
    }

    msum->sum = 0;
    msum->index = 0;
    msum->count = 0;
    msum->num_readings = num_readings;
}

void MovingSum_update(MovingSum* msum, uint16_t new_value) {
    msum->sum -= msum->readings[msum->index];
    msum->readings[msum->index] = new_value;
    msum->sum += new_value;

    msum->index = (msum->index + 1) % msum->num_readings;

    if (msum->count < msum->num_readings) {
        msum->count++;
    }
}

#endif

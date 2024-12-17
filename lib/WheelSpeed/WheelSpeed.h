#ifndef WHEELSPEED_H
#define WHEELSPEED_H

#include <Arduino.h>
#include <ASTCanLib.h>

#define MESSAGE_PROTOCOL  0         // 0: CAN 2.0A, 1: CAN 2.0B
#define MESSAGE_LENGTH    8         // Data length

#define ROLLING_RADIUS 900          // hundredths of inches
#define SPOKES 16

#define SCALED_PI 31416             // scaled by 10^4 to avoid floating point arithmetic
#define SCALED_MICROS_IN_HR 3600000 // scaled by 10^-3
#define SCALED_HNTH_IN_IN_MILE 63360 // scaled by 10^-2

// Final answer will be scaled by 10^3 -> 3 decimal places of accuracy
#define MASTER_CONST ((uint64_t(SCALED_MICROS_IN_HR) * 2 * SCALED_PI * ROLLING_RADIUS) / (SPOKES * SCALED_HNTH_IN_IN_MILE)) 

class WheelSpeedSensor {

private:
    uint8_t pin;

    volatile uint8_t count;
    volatile uint32_t start_time;
    volatile uint32_t end_time;
    bool reset_flag;

    st_cmd_t tx_msg;
    uint8_t tx_buffer[MESSAGE_LENGTH];

public:
    void inc();
    WheelSpeedSensor() : count(0), start_time(0), end_time(0), reset_flag(false) {
        memset(tx_buffer, 0, sizeof(tx_buffer));
    }
    void init(uint8_t pin, void(*intFun)(), uint16_t id);
    void tx();
};

#endif

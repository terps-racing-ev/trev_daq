#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/* CAN Info */
#define CAN_BAUD_RATE 250000
#define CAN_PROTOCOL 0
#define CAN_MESSAGE_SIZE 8


#define MILLIS_IN_SEC   1000

/* Front Right Board */
#define FR_BOARD_CAN_ID 0xF2
#define FR_BOARD_FREQ   50

#define B_BP_PIN        A0
#define PITOT_PIN       A1
#define SA_PIN          A2

/* Cooling Board */
#define COOLING_CAN_ID  0xCB
#define BR_BOARD_FREQ   2

#define NUM_CT_SENSORS  4
#define BASE_CT_PIN     A0
#define FLOW_PIN        A6

#define PWM_PIN         5

/* Back Left Board */
#define BL_BOARD_CAN_ID 0xB1
#define BL_BOARD_FREQ   50

#define BR_LP_PIN       A0
#define BL_LP_PIN       A1

#define BR_WSP_PIN      6
#define BL_WSP_PIN      7

/* Front Left Board */
#define FL_BOARD_CAN_ID 0xF1
#define FL_BOARD_FREQ   50

#define FR_LP_PIN       A1
#define FL_LP_PIN       A3

#define FR_WSP_PIN      6
#define FL_WSP_PIN      7


#endif

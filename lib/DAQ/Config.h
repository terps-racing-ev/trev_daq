#ifndef CONFIG_H
#define CONFIG_H


// Multi frequency sensors on a board can be easily implemented
#define MILLIS_IN_SEC   1000

/* Front Right Board */
#define FR_BOARD_FREQ   50

#define B_BP_PIN        A0
#define B_BP_CAN_ID     0x6A

#define PITOT_PIN       A1
#define PITOT_CAN_ID    0x7A

#define SA_PIN          A2
#define SA_CAN_ID       0x8A

/* Back Right Board (Cooling) */
#define BR_BOARD_FREQ   2

#define BASE_CT_PIN     A0
#define CT_CAN_ID       0x1B

#define FLOW_PIN        A6
#define FLOW_CAN_ID     0x2B

#define PWM_PIN         5

/* Back Left Board */
#define BL_BOARD_FREQ   50

#define BR_LP_PIN       A0
#define BL_LP_PIN       A1
#define B_LP_CAN_ID     0x5C

#define BR_WSP_PIN      6
#define BL_WSP_PIN      7
#define B_WSP_CAN_ID    0x4C

/* Front Left Board */
#define FL_BOARD_FREQ   50

#define FR_LP_PIN       A1
#define FL_LP_PIN       A3
#define F_LP_CAN_ID     0x5D

#define FR_WSP_PIN      6
#define FL_WSP_PIN      7
#define F_WSP_CAN_ID    0x4D


#endif

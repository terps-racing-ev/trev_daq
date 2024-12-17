#include "CoolantTemp.h"

///////// Variables to change
int VPIN = A3;                  //5V pin
int Sensor = A0;                //Sesnor Pin

// Stienhart Equation Constants, see matlab file to adjust
float A = 0.000689287402357408;
float B = 0.000289807635936112;
float C = -0.0000000158833675;

float Resistor = 10030;         //Resistor used in the circuit (ohms)

int Period = 2000;              //How long between each sensor reading (ms)

int id = 0x12;                  //CAN id
//////////////////////////////


#define MESSAGE_ID        256       // Message ID
#define MESSAGE_PROTOCOL  1         // CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
#define MESSAGE_LENGTH    8         // Data length: 8 bytes
#define MESSAGE_RTR       0         // rtr bit

st_cmd_t txMsg;

float voltage = 0;
float resistance = 0;
float tempC = 0;
uint8_t txBuffer[8] = {};
void setup() {
    pinMode(VPIN,OUTPUT);
    pinMode(Sensor,INPUT);

    canInit(500000);                  // Initialise CAN port. must be before Serial.begin
    Serial.begin(1000000);             // start serial port
    txMsg.pt_data = &txBuffer[0];      // reference message data to transmit buffer
}

void loop() {
    analogWrite(VPIN, 1023);
    voltage = analogRead(Sensor) * (5.0 / 1023.0);
    resistance = voltage * Resistor / (5.00 - voltage);
    tempC = pow((A + B * log(resistance) + C * pow(log(resistance), 3)), -1) - 273.15;

    // Convert float to bytes and send over CAN
    memcpy(txBuffer, &tempC, sizeof(tempC));

      // Setup CAN packet.
    txMsg.ctrl.ide = MESSAGE_PROTOCOL;  // Set CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
    txMsg.id.ext   = MESSAGE_ID;        // Set message ID
    txMsg.dlc      = MESSAGE_LENGTH;    // Data length: 8 bytes
    txMsg.ctrl.rtr = MESSAGE_RTR;       // Set rtr bit

    // Send command to the CAN port controller
    txMsg.cmd = CMD_TX_DATA;       // send message
    // Wait for the command to be accepted by the controller
    while(can_cmd(&txMsg) != CAN_CMD_ACCEPTED);
    // Wait for command to finish executing
    while(can_get_status(&txMsg) == CAN_STATUS_NOT_COMPLETED);
    // Transmit is now complete. Wait a bit and loop

    delay(Period);
}
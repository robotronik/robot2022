

#include <Arduino.h>
#include <Wire.h>


#define I2C_ADDRESS_OTHER 40
#define I2C_ADDRESS_ME 50

//reception
#define CMDCHOIXDIRECTION   20
//envoie
#define AVANT               21
#define ARIERRE             22
#define AVANTARIERRE        23

typedef struct
{
    uint16_t cmd;
    uint16_t distance;
}i2CReceive;

typedef union
{
    i2CReceive structR;
    uint8_t tabR[4];
}i2CReceiveUnion;


void i2cSetup(void);
void receiveI2C(int howMany);
void sendDistanceAv(uint16_t d);
void sendDistanceAr(uint16_t d);
int getDirection(void);



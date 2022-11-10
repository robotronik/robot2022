

#include <Arduino.h>
#include <Wire.h>


#define I2C_ADDRESS_OTHER 120
#define I2C_ADDRESS_ME 40

#define SETPOSITOION        1
#define LINEAIRE            10
#define ANGULAIRE           11
#define TOURNERAVANCER      12
#define STOPMOTOR           13
#define TOURNER             14

typedef struct
{
    uint16_t cmd;
    uint16_t x;
    uint16_t y;
    uint16_t r;

}i2CReceive;

typedef union
{
    i2CReceive structR;
    uint8_t tabR[8];
}i2CReceiveUnion;



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
}i2CReceiveUltrason;

typedef union
{
    i2CReceiveUltrason structR;
    uint8_t tabR[4];
}i2CReceiveUltrasonUnion;



void i2cSetup(void);
void receiveI2C(int howMany);
void envoieCommande(uint16_t cmd, uint16_t x, uint16_t y, uint16_t z);
void stopCommande(void);
bool movementIsFinish(void);

int getdistanceAsserv(void);
int getdistanceObstacleAvant(void);
int getdistanceObstacleArriere(void);



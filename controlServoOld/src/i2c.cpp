#include "i2c.h"

i2CReceiveUnion envoi;
bool bMouvementFini = false;
uint16_t distanceAsserv;
uint16_t distanceObstacleAvant;
uint16_t distanceObstacleArriere;

void i2cSetup(){
    Wire.begin(I2C_ADDRESS_ME);
    Wire.onReceive(receiveI2C);

    Wire.beginTransmission(I2C_ADDRESS_OTHER);
    Wire.write("TEST");
    Wire.endTransmission();
}

void receiveI2C(int howMany) {
    i2CReceiveUltrasonUnion buffer;
    for(int i = 0; i<4;i++){
        buffer.tabR[i]=0;
    }
    int i = 0;
    while (Wire.available() > 0) {
        char c = Wire.read();
        if(i<4){
            buffer.tabR[i]=c;
            i++;
        }
        //Serial.print(c);
    }
    if(buffer.structR.cmd == 70){
        // Serial.print("c'est Fini");
        bMouvementFini = true;
    }
    if(buffer.structR.cmd == 71){
        distanceAsserv = (uint16_t)buffer.structR.distance;
        //Serial.println(buffer.structR.distance);
    }
    if(buffer.structR.cmd==AVANT){
        distanceObstacleAvant = buffer.structR.distance;
    }
    if(buffer.structR.cmd==ARIERRE){
        distanceObstacleArriere = buffer.structR.distance;
    }
}

void envoieCommande(uint16_t cmd, uint16_t x, uint16_t y, uint16_t z){
    bMouvementFini = false;
    envoi.structR.cmd = (uint16_t)cmd;
    envoi.structR.x = (uint16_t)x;
    envoi.structR.y = (uint16_t)y;
    envoi.structR.r = (uint16_t)z;
    Wire.beginTransmission(I2C_ADDRESS_OTHER);
    Wire.write(envoi.tabR,8);
    Wire.endTransmission();
    
    distanceAsserv = 10000;
}

void stopCommande(void){
    Wire.beginTransmission(I2C_ADDRESS_OTHER);
    envoieCommande(STOPMOTOR, 0, 0, 0);
    Wire.endTransmission();
}

bool movementIsFinish(void){
    bool bret = false;
    if(bMouvementFini==true){
        bMouvementFini = false;
        bret = true;
    }
    return bret;
}

int getdistanceAsserv(void){
    return abs((int)distanceAsserv);
}
int getdistanceObstacleAvant(void){
    return abs((int)distanceObstacleAvant);
    //return 1000;
}
int getdistanceObstacleArriere(void){
    return abs((int)distanceObstacleArriere);
    //return 1000;
}

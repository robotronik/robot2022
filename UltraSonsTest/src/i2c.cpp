#include "i2c.h"

i2CReceiveUnion envoi;
int direction = AVANTARIERRE;

void i2cSetup(){
    Wire.begin(I2C_ADDRESS_ME);
    Wire.onReceive(receiveI2C);
}

void receiveI2C(int howMany) {
    char buffer[4];
    for(int i = 0; i<4;i++){
        buffer[i]=0;
    }
    int i = 0;
    while (Wire.available() > 0) {
        char c = Wire.read();
        if(i<4){
            buffer[i]=c;
            i++;
        }
        Serial.print(c);
    }
    if(i>=2){
        if(buffer[0]==CMDCHOIXDIRECTION){
            direction = buffer[1];
        }
    }
    
 Serial.println();
}

void sendDistanceAv(uint16_t d){
    envoi.structR.cmd = AVANT;
    envoi.structR.distance = d;
    Wire.beginTransmission(I2C_ADDRESS_OTHER);
    Wire.write(envoi.tabR,4);
    Wire.endTransmission();
}

void sendDistanceAr(uint16_t d){
    envoi.structR.cmd = ARIERRE;
    envoi.structR.distance = d;
    Wire.beginTransmission(I2C_ADDRESS_OTHER);
    Wire.write(envoi.tabR,4);
    Wire.endTransmission();
}

int getDirection(void){
    return direction;
    //return 21;
}



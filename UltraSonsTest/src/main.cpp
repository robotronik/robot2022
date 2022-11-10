#include <Arduino.h>
#include "i2c.h"

#define BUFFERSIZE 3
#define NBCAPTEUR 5

const int pingPin0 = 12; // Trigger Pin of Ultrasonic Sensor
const int echoPin0 = 11; // Echo Pin of Ultrasonic Sensor

const int pingPin1 = 10; // Trigger Pin of Ultrasonic Sensor
const int echoPin1 = 9; // Echo Pin of Ultrasonic Sensor

const int pingPin2 = 8; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 7; // Echo Pin of Ultrasonic Sensor

const int pingPin3 = 6; // Trigger Pin of Ultrasonic Sensor
const int echoPin3 = 5; // Echo Pin of Ultrasonic Sensor

const int pingPin4 = 4; // Trigger Pin of Ultrasonic Sensor
const int echoPin4 = 3; // Echo Pin of Ultrasonic Sensor

int PinTab[NBCAPTEUR][2] = {{pingPin0,echoPin0},{pingPin1,echoPin1},{pingPin2,echoPin2},{pingPin3,echoPin3},{pingPin4,echoPin4}};

long buffer[NBCAPTEUR][BUFFERSIZE];
long buffercopie[NBCAPTEUR][BUFFERSIZE];

 unsigned long timePrecedent;


long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void bufferInit(void){
   for(int i = 0; i<NBCAPTEUR; i++){
      for(int j = 0; j<BUFFERSIZE;j++){
         buffer[i][j] = 0;
      }
   }
}

void BuffertAddValue(int icapteur, long value){
   for(int i = BUFFERSIZE-1; i>0; i--){
      buffer[icapteur][i] = buffer[icapteur][i-1];
   }
   buffer[icapteur][0] = value;
}

long BuffertDistance(int capteur){
   //Je fais une moyenne
    long somme = 0;
   for(int i = 0; i<BUFFERSIZE;i++){
      somme += buffer[capteur][i];
   }
   long moyenne = somme/BUFFERSIZE;

   //je garde la valeur la plus proche de la moyenne
   long valeur = buffer[capteur][0];
   for(int i = 1; i<BUFFERSIZE;i++){
      if( abs(valeur - moyenne) > abs(buffer[capteur][i] - moyenne)){
         valeur = buffer[capteur][i];
      }
   }
   return valeur;
}

void printBuffer(void){
    for(int i = 0; i<NBCAPTEUR; i++){
      for(int j = 0; j<BUFFERSIZE;j++){
         Serial.print(buffer[i][j]);
         Serial.print("    ");
      }
      Serial.println();
   }
}

void copieBuffer(int icpateur){
   for(int j = 0; j<BUFFERSIZE;j++){
      buffercopie[icpateur][j] = buffer[icpateur][j];
   }
}

void triBuffer(int icpateur){
   long copie;
   for(int k = 0; k<BUFFERSIZE-1;k++){
      for(int j = 0; j<BUFFERSIZE-1;j++){
         if(buffercopie[icpateur][j]>buffercopie[icpateur][j+1]){
            copie = buffercopie[icpateur][j];
            buffercopie[icpateur][j] = buffercopie[icpateur][j+1];
            buffercopie[icpateur][j+1] = copie;
         }
      }
   }
}

void printBufferCopie(void){
   int i;
   for(int i = 0; i<NBCAPTEUR; i++){
      for(int j = 0; j<BUFFERSIZE;j++){
         Serial.print(buffercopie[i][j]);
         Serial.print("    ");
      }
      Serial.println();
   }
}

long bufferMediane(int icpateur){
   copieBuffer(icpateur);
   triBuffer(icpateur);
   return buffer[icpateur][(BUFFERSIZE+1)/2];
}

long mesureDistance(int icapteur){
   long duration, cm;
   digitalWrite(PinTab[icapteur][0], HIGH);
   delayMicroseconds(10);
   digitalWrite(PinTab[icapteur][0], LOW);
   duration = pulseIn(PinTab[icapteur][1], HIGH);
   cm = microsecondsToCentimeters(duration);
   BuffertAddValue(icapteur,cm);
   cm = bufferMediane(icapteur);
   return cm;
   
}

void setup() {
   Serial.begin(9600); // Starting Serial Terminal

   pinMode(pingPin0, OUTPUT);
   pinMode(echoPin0, INPUT);
   digitalWrite(pingPin0, LOW);

   pinMode(pingPin1, OUTPUT);
   pinMode(echoPin1, INPUT);
   digitalWrite(pingPin1, LOW);

   pinMode(pingPin2, OUTPUT);
   pinMode(echoPin2, INPUT);
   digitalWrite(pingPin2, LOW);

   pinMode(pingPin3, OUTPUT);
   pinMode(echoPin3, INPUT);
   digitalWrite(pingPin3, LOW);
   delayMicroseconds(50);

   pinMode(pingPin4, OUTPUT);
   pinMode(echoPin4, INPUT);
   digitalWrite(pingPin4, LOW);
   delayMicroseconds(50);

   i2cSetup();
   bufferInit();
}

void loop() {
      long distance0;
      long distance1;
      long distance2;
      long distance3;
      long distance4;

      timePrecedent = millis()+30;
      if(getDirection()==AVANT || getDirection()==AVANTARIERRE){
         distance4 = mesureDistance(4);
      }
      if(getDirection()==ARIERRE || getDirection()==AVANTARIERRE){
         distance2 = mesureDistance(2);
      }
      while (timePrecedent>millis());


       timePrecedent = millis()+30;
      if(getDirection()==AVANT || getDirection()==AVANTARIERRE){
         distance0 = mesureDistance(0);
         distance1 = mesureDistance(1);
         distance0 *= 1;
         distance1 *= 1;
      }
      if(getDirection()==ARIERRE || getDirection()==AVANTARIERRE){
         distance3 = mesureDistance(3);
      }
      while (timePrecedent>millis());

      if(getDirection()==AVANT || getDirection()==AVANTARIERRE){
          if(distance0<distance1 && distance0<distance4){
            Serial.print(distance0);
            Serial.print("   ");
            sendDistanceAv((uint16_t)distance0);
          }
          if(distance1<distance4 && distance1<distance0){
            Serial.print(distance1);
            Serial.print("   ");
            sendDistanceAv((uint16_t)distance1);
          }
          if(distance4<distance0 && distance4<distance1){
            Serial.print(distance4);
            Serial.print("   ");
            sendDistanceAv((uint16_t)distance4);
          }
      }

      if(getDirection()==ARIERRE || getDirection()==AVANTARIERRE){
         if(distance2<distance3){
            Serial.print(distance2);
            Serial.print("   ");
            sendDistanceAr((uint16_t)distance2);
          }
          else {
            Serial.print(distance3);
            Serial.print("   ");
            sendDistanceAr((uint16_t)distance3);
          }
      }
      Serial.println();
      Serial.print(distance0);
      Serial.print("   ");
      Serial.print(distance1);
      Serial.print("   ");
      Serial.print(distance2);
      Serial.print("   ");
      Serial.print(distance3);
      Serial.print("   ");
      Serial.println(distance4);
}






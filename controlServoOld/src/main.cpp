#include <Arduino.h>


#include <Wire.h>
#include <Servo.h>
#include "i2c.h"
#include "RobotServo.h"

#define PINBUTTONSELECTCOLOR 5
#define PINBUTTONRED 7
#define PINBUTTONGREEN 6
#define PINDEMAREUR 8
#define PINTESTRESISTANCE A1

#define RESISTANCEVIOLET 1
#define RESISTANCEJAUNE 2
#define RESISTANCENON 3
#define RESISTANCERIEN 0

#define POSICARREFOUILE 700
#define POSICARREFOUILEV 650

int i;
int iCommande;
Servo myservo;
unsigned long timeOutCommande;
unsigned long timeOutParty;
unsigned long timeOutRentrer;

void commande(int cmd,int x,int y,int alpha,int fin, int directionUltrasons, int distanceStop = 40);
void commandeTimeOut(int cmd,int x,int y,int alpha,int fin, int directionUltrasons, int distanceStop,int timeOut);

void detectfin(){
  if(millis()>timeOutParty){
    stopCommande();
    Serial.println("Fini");
    while (1)
    {
      delay(1);
    }
    
  }
  if(millis()>timeOutRentrer){
    timeOutRentrer = millis()+10000000;
    if(digitalRead(PINBUTTONSELECTCOLOR) == 0){
      commande(TOURNER,675,150,0,50,-1);
      commandeTimeOut(LINEAIRE,675,150,135,50,0,20,4000);
      stopCommande();
    }
    else{
      commande(TOURNER,675,3000-150,0,50,-1);
      commandeTimeOut(LINEAIRE,675,3000-150,135,50,0,40,4000);
      stopCommande();
    }
    Serial.println("FiniRentrer");
    while (1)
    {
      delay(1);
    }
    
  }
}

//0 pour avant 1 pour arrière
void commande(int cmd,int x,int y,int alpha,int fin, int directionUltrasons, int distanceStop = 40){
  bool bstop = false;
  delay(10);
  envoieCommande(cmd,x,y,alpha);
  //delay(75);
  while(getdistanceAsserv()<fin);
  delay(100);
  if(directionUltrasons == 0){
    do{
      if(getdistanceObstacleAvant()>distanceStop && bstop==true){
        envoieCommande(cmd,x,y,alpha);
        delay(1);
        envoieCommande(cmd,x,y,alpha);
        bstop = false;
      }
      else if(getdistanceObstacleAvant()<=distanceStop && bstop == false){
        stopCommande();
        delay(1);
        stopCommande();
        bstop = true;
      }
      delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin);
  }
  else if(directionUltrasons == 1){
    do{
      if(getdistanceObstacleArriere()>distanceStop && bstop==true){
        envoieCommande(cmd,x,y,alpha);
        delay(1);
        envoieCommande(cmd,x,y,alpha);
        bstop = false;
      }
      else if(getdistanceObstacleArriere()<=distanceStop && bstop == false){
        stopCommande();
        delay(1);
        stopCommande();
        bstop = true;
      }
      delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin);
  }
  else{
    envoieCommande(cmd,x,y,alpha);
    do{
      delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin);
  }
}

int testResistance(void){
  int val = analogRead(A1);
  if(val<409){
    delay(1);
    val = analogRead(A1);
    if(val<409){
      return RESISTANCEVIOLET;
    }
  }
  if(val<682){
    delay(1);
    val = analogRead(A1);
    if(val<682 && val>=409){
      return RESISTANCEJAUNE;
    }
  }
  if(val<938){
    delay(1);
    val = analogRead(A1);
    if(val<938 && val>=682){
      return RESISTANCENON;
    }
  }
  return RESISTANCERIEN;
}

int commandeTestResitance(int cmd,int x,int y,int alpha,int fin, int directionUltrasons, int distanceStop,int timeOut){
  bool bstop = false;
  delay(10);
  envoieCommande(cmd,x,y,alpha);
  while(getdistanceAsserv()<fin);
  delay(100);
  int valResistance;
  timeOutCommande = millis()+timeOut;
  if(directionUltrasons == 0){
    do{
      if(getdistanceObstacleAvant()>distanceStop && bstop==true){
        envoieCommande(cmd,x,y,alpha);
        delay(1);
        envoieCommande(cmd,x,y,alpha);
        bstop = false;
      }
      else if(getdistanceObstacleAvant()<=distanceStop && bstop == false){
        stopCommande();
        delay(1);
        stopCommande();
        bstop = true;
      }
      valResistance = testResistance();
      //delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin && valResistance == RESISTANCERIEN && timeOutCommande>millis());
  }
  else if(directionUltrasons == 1){
    do{
      if(getdistanceObstacleArriere()>distanceStop && bstop==true){
        envoieCommande(cmd,x,y,alpha);
        delay(1);
        envoieCommande(cmd,x,y,alpha);
        bstop = false;
      }
      else if(getdistanceObstacleArriere()<=distanceStop && bstop == false){
        stopCommande();
        delay(1);
        stopCommande();
        bstop = true;
      }
      valResistance = testResistance();
      //delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin && valResistance == RESISTANCERIEN && timeOutCommande>millis());
  }
  else{
    envoieCommande(cmd,x,y,alpha);
    do{
      delay(1);
      valResistance = testResistance();
      delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin && valResistance == RESISTANCERIEN && timeOutCommande>millis());
  }
  return valResistance;
}

void commandeTimeOut(int cmd,int x,int y,int alpha,int fin, int directionUltrasons, int distanceStop,int timeOut){
  bool bstop = false;
  delay(10);
  envoieCommande(cmd,x,y,alpha);
  while(getdistanceAsserv()<fin);
  delay(100);
  timeOutCommande = millis()+timeOut;
  if(directionUltrasons == 0){
    do{
      if(getdistanceObstacleAvant()>distanceStop && bstop==true){
        envoieCommande(cmd,x,y,alpha);
        delay(1);
        envoieCommande(cmd,x,y,alpha);
        bstop = false;
      }
      else if(getdistanceObstacleAvant()<=distanceStop && bstop == false){
        stopCommande();
        delay(1);
        stopCommande();
        bstop = true;
      }
      //delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin && timeOutCommande>millis());
  }
  else if(directionUltrasons == 1){
    do{
      if(getdistanceObstacleArriere()>distanceStop && bstop==true){
        envoieCommande(cmd,x,y,alpha);
        delay(1);
        envoieCommande(cmd,x,y,alpha);
        bstop = false;
      }
      else if(getdistanceObstacleArriere()<=distanceStop && bstop == false){
        stopCommande();
        delay(1);
        stopCommande();
        bstop = true;
      }
      //delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin);
  }
  else{
    envoieCommande(cmd,x,y,alpha);
    do{
      delay(1);
      detectfin();
    }while(getdistanceAsserv()>fin && timeOutCommande>millis());
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println("DemarageArduino");

  i2cSetup();
  myservo.attach(PINSERVOSTATUETTE);
  myservo.write(HAUTEUR_PIEDESTAL);

  pinMode(PINBUTTONSELECTCOLOR,INPUT);
  pinMode(PINBUTTONRED,INPUT);
  pinMode(PINBUTTONGREEN,INPUT);
  pinMode(PINDEMAREUR,INPUT);
  pinMode(PINTESTRESISTANCE,INPUT);

    //test Bouton
  if(digitalRead(PINBUTTONSELECTCOLOR)){Serial.print("PINBUTTONSELECTCOLOR : on   ");}else{Serial.print("PINBUTTONSELECTCOLOR : off   ");}
  if(digitalRead(PINBUTTONRED)){Serial.print("PINBUTTONRED : on   ");}else{Serial.print("PINBUTTONRED : off   ");}
  if(digitalRead(PINBUTTONGREEN)){Serial.print("PINBUTTONGREEN : on   ");}else{Serial.print("PINBUTTONGREEN : off   ");}
  if(digitalRead(PINDEMAREUR)){Serial.print("PINDEMAREUR : on   ");}else{Serial.print("PINDEMAREUR : off   ");}


  timeOutParty = millis()+10000000; //départ 100secondes
  timeOutRentrer = millis()+10000000;
  //delay(2000);//attente démarage

  while ((digitalRead(PINBUTTONGREEN) == 0) || (digitalRead(PINDEMAREUR) == 0));
  delay(1000);

  if(digitalRead(PINBUTTONSELECTCOLOR) == 0){

    envoieCommande(SETPOSITOION,1690,330,135);
    delay(100);

    //départ -> zone de repos
    commande(LINEAIRE,1640,380,135,10,1); //fonction,x,y,angle,precision,capteurs(1=arriere, 0= avant, sinon ignorer)
    Serial.println("OkLin");
    commande(TOURNER,700,160,0,5,-1);
    Serial.println("OkTourn");
    commande(LINEAIRE,700,160,0,10,0);
    commande(ANGULAIRE,700,160,0,5,-1);
    stopCommande();
    while( (digitalRead(PINDEMAREUR) == 1)){ 
    }

    timeOutParty = millis()+100000; //départ 100secondes
    timeOutRentrer = millis()+85000;

    //zone de repos -> chercher statuette
    delay(500);
    commande(TOURNER,1640,300,0,10,-1);
    commande(LINEAIRE,1640,380,135,5,0);
    commande(TOURNER,1690,330,0,2,-1);
    commandeTimeOut(LINEAIRE,1710,310,0,4,0,20,2000);
    stopCommande();
    myservo.write(HAUTEUR_MAX); //commande pour monter les bras (hauteurmax = montée, hauteurmin = descente)
    delay(500);

    //ramener statuette
    commande(LINEAIRE,1540,480,135,50,1);
    commande(TOURNER,200,225,0,10,-1);
    commande(LINEAIRE,200,225,0,50,0);
    //delay(1000);
    commande(TOURNER,90,225,0,5,-1);
    commandeTimeOut(LINEAIRE,40,225,135,10,0,20,2000);
    stopCommande();
    myservo.write(HAUTEUR_MIN);
    delay(500);
    // statuette posée

    // direction carrées de fouille
    commande(LINEAIRE,200,225,135,50,1);
    myservo.write(HAUTEUR_CARRE); 
  int val;
  for(i=0;i<7;i++){
    commande(TOURNER,1745,(POSICARREFOUILE+i*DECALAGE),0,10,-1);
    commande(LINEAIRE,1745,(POSICARREFOUILE+i*DECALAGE),135,20,0);
    commande(TOURNER,2000,(POSICARREFOUILE+i*DECALAGE),0,2,-1);
    if(i!=1){
      val = commandeTestResitance(LINEAIRE,2000,(POSICARREFOUILE+i*DECALAGE),135,5,0,40,1500);
      commande(LINEAIRE,1745,(POSICARREFOUILE+i*DECALAGE),135,50,1);
    }

    if(i==1 || val==RESISTANCEJAUNE){
      myservo.write(HAUTEUR_POUSSECARRE); //commande pour monter les bras (hauteurmax = montée, hauteurmin = descente)
      commandeTimeOut(LINEAIRE,2000,(POSICARREFOUILE+i*DECALAGE),135,4,0,40,2000);
      myservo.write(HAUTEUR_CARRE);
      commande(LINEAIRE,1745,(POSICARREFOUILE+i*DECALAGE),135,10,1);
      }
  }
    // boucle pour tester tous les carrés
  commande(TOURNER,675,1790,0,50,-1);
  commande(LINEAIRE,675,1790,135,50,0);
  commande(TOURNER,675,150,0,50,-1);
  commandeTimeOut(LINEAIRE,675,150,135,50,0,20,4000);
  stopCommande();

  }
  else{

    envoieCommande(SETPOSITOION,1690,3000-330,45);
    delay(100);

    //départ -> zone de repos
    commande(LINEAIRE,1640,3000-380,135,10,1); //fonction,x,y,angle,precision,capteurs(1=arriere, 0= avant, sinon ignorer)
    Serial.println("OkLin");
    commande(TOURNER,700,3000-160,0,5,-1);
    Serial.println("OkTourn");
    commande(LINEAIRE,700,3000-160,0,10,0);
    commande(ANGULAIRE,700,3000-160,180,5,-1);
    stopCommande();
    while( (digitalRead(PINDEMAREUR) == 1)){ 
    }

    timeOutParty = millis()+100000; //départ 100secondes

    //zone de repos -> chercher statuette
    delay(500);
    commande(TOURNER,1640,3000-300,0,10,-1);
    commande(LINEAIRE,1640,3000-380,135,3,0);
    commande(TOURNER,1700,3000-310,0,1,-1);
    commandeTimeOut(LINEAIRE,1700,3000-310,0,4,0,40,2000);
    stopCommande();
    myservo.write(HAUTEUR_MAX); //commande pour monter les bras (hauteurmax = montée, hauteurmin = descente)
    delay(500);

    //ramener statuette
    commande(LINEAIRE,1540,3000-480,135,50,1);
    commande(TOURNER,200,3000-225,0,10,-1);
    commande(LINEAIRE,200,3000-225,0,50,0);
    //delay(1000);
    commande(TOURNER,90,3000-225,0,5,-1);
    commandeTimeOut(LINEAIRE,40,3000-225,135,10,0,20,2000);
    stopCommande();
    myservo.write(HAUTEUR_MIN);
    delay(500);
    // statuette posée

    // direction carrées de fouille
    commande(LINEAIRE,200,3000-225,135,50,1);
    myservo.write(HAUTEUR_CARRE); 

    int val;
    for(i=0;i<7;i++){
      commande(TOURNER,1745,3000-(POSICARREFOUILEV+i*DECALAGE),0,10,-1);
      commande(LINEAIRE,1745,3000-(POSICARREFOUILEV+i*DECALAGE),135,20,0);
      commande(TOURNER,2000,3000-(POSICARREFOUILEV+i*DECALAGE),0,2,-1);
      if(i!=1){
        val = commandeTestResitance(LINEAIRE,2000,3000-(POSICARREFOUILEV+i*DECALAGE),135,5,0,40,1500);
        commande(LINEAIRE,1745,3000-(POSICARREFOUILEV+i*DECALAGE),135,50,1);
      }

      if(i==1 || val==RESISTANCEVIOLET){
        myservo.write(HAUTEUR_POUSSECARRE); //commande pour monter les bras (hauteurmax = montée, hauteurmin = descente)
        commandeTimeOut(LINEAIRE,2000,3000-(POSICARREFOUILEV+i*DECALAGE),135,4,0,40,2000);
        myservo.write(HAUTEUR_CARRE);
        commande(LINEAIRE,1745,3000-(POSICARREFOUILEV+i*DECALAGE),135,10,1);
      }
      
    }
      // boucle pour tester tous les carrés
    commande(TOURNER,675,3000-1790,0,50,-1);
    commande(LINEAIRE,675,3000-1790,135,50,0);
    commande(TOURNER,675,3000-150,0,50,-1);
    commandeTimeOut(LINEAIRE,675,3000-150,135,50,0,40,4000);
    stopCommande();

  }

  



  while (1)
  {
    /* code */
  }
  

}


void loop() {

}



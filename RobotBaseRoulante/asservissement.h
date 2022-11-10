#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <math.h>
#include "math_precalc.h"
#include "variable.h"
#include "uart.h"
#include "clock.h"
#include "RoueCodeuse.h"
#include "moteur.h"
#include "Test.h"


#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define FREQUENCE 20.0
#define PERIODE (1.0/FREQUENCE)

#define DISTANCEFREINAGE 1
#define ANGLEFREINAGE 1
#define DISTNACEFREEZE 20
#define TOURNERAVANCERCHANGE 0.5



#define KP_VITESSEANGULAIRE 1               
#define KI_VITESSEANGULAIRE (6*PERIODE)           
#define KI_VITESSEANGULAIREPECISION (6*PERIODE)
#define ANGLELIMITPRECISION 2 
#define KD_VITESSEANGULAIRE 0

#define KP_POSITIONANGULAIRE 2.5
#define KI_POSITIONANGULAIRE 0
#define KD_POSITIONANGULAIRE 0
#define VITESSEMAXANGULAIRE 250
#define ACCELERATIONMAXANGULAIRE 30

#define KP_VITESSELINEAIRE 0.8                  //0.8
#define KI_VITESSELINEAIRE (1*0.05)
#define KD_VITESSELINEAIRE 0   //-(0.008/0.05)        //0

#define KP_POSITIONLINEAIRE 1.2   //Baisse angle 2 vers 1.5
#define KI_POSITIONLINEAIRE 0
#define KD_POSITIONLINEAIRE 0
#define VITESSEMAXLINEAIRE 600
#define ACCELERATIONMAXLINEAIRE 60


typedef enum{AVANCER = 0,TOURNER,TOURNERAVANCER,TOURNERPOINT}TYPEASSERVE;
typedef enum{AVANCE,TOURNE,STOP}MODETOURNERAVANCER;



void setConsigneLineaire(coordonne consigne);
void setConsigneAngulaire(coordonne consigne);
void setConsigneTournerAvancer(coordonne consigne);
void setConsigneTourner(coordonne consigne);
void stopMoteur(void);
void printAsserv(void);
void asservisement_setup(void);
void resetAsserv(void);

void asservSetCallback(void (*f)(void));
void asservFinish(void);

void asservSetDistanceCallback(void (*f)(int));
void asservDistance(int distance);

double getinfo(void);
double getinfo2(void);